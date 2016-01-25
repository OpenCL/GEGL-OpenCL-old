/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 */


#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

property_color  (fill, _("Fill Color"), "rgba(0.0,0.0,0.0,0.6)")
  description(_("Color of paint to use for filling, use 0 opacity to disable filling"))

property_color  (stroke, _("Stroke Color"), "rgba(0.0,0.0,0.0,0.0)")
  description(_("Color of paint to use for stroking"))

property_double (stroke_width,_("Stroke width"), 2.0)
  description (_("The width of the brush used to stroke the path"))
  value_range (0.0, 200.0)

property_double (stroke_opacity, _("Stroke opacity"), 1.0)
  description (_("Opacity of stroke, note, does not behave like SVG since at the moment stroking is done using an airbrush tool"))
  value_range (-2.0, 2.0)

property_double (stroke_hardness, _("Hardness"), 0.6)
  description (_("Hardness of the brush, 0.0 for a soft brush, 1.0 for a hard brush"))
  value_range (0.0, 1.0)

property_string (fill_rule,_("Fill rule"), "nonzero")
  description(_("How to determine what to fill (nonzero|evenodd)"))

property_string (transform,_("Transform"), "")
  description(_("SVG style description of transform"))

property_double (fill_opacity, _("Fill opacity"), 1.0)
  description(_("The fill opacity to use"))
  value_range (-2.0, 2.0)

property_path (d, _("Vector"), NULL)
  description (_("A GeglVector representing the path of the stroke"))

#else

#define GEGL_OP_FILTER
#define GEGL_OP_C_SOURCE path.c

#include "gegl-plugin.h"
#include "gegl-path.h"
static void path_changed (GeglPath *path,
                          const GeglRectangle *roi,
                          gpointer userdata);

#include "gegl-op.h"
#include <cairo.h>
#include <math.h>

typedef struct StampStatic {
  gboolean    valid;
  const Babl *format;
  gfloat     *buf;
  gdouble     radius;
}StampStatic;

static void gegl_path_stroke  (GeglBuffer *buffer,
                               const GeglRectangle *clip_rect,
                               GeglPath *vector,
                               GeglColor  *color,
                               gdouble     linewidth,
                               gdouble     hardness,
                               gdouble     opacity);

static void gegl_path_stamp   (GeglBuffer *buffer,
                               const GeglRectangle *clip_rect,
                               gdouble     x,
                               gdouble     y,
                               gdouble     radius,
                               gdouble     hardness,
                               GeglColor  *color,
                               gdouble     opacity);

static void
gegl_path_stroke (GeglBuffer *buffer,
                  const GeglRectangle *clip_rect,
                  GeglPath *vector,
                  GeglColor  *color,
                  gdouble     linewidth,
                  gdouble     hardness,
                  gdouble     opacity)
{
  gfloat traveled_length = 0;
  gfloat need_to_travel = 0;
  gfloat x = 0,y = 0;
  GeglPathList *iter;
  gdouble       xmin, xmax, ymin, ymax;
  GeglRectangle extent;

  if (!vector)
    return;

  if (!clip_rect)
    {
      g_print ("using buffer extent\n");
      clip_rect = gegl_buffer_get_extent (buffer);
    }

  iter = gegl_path_get_flat_path (vector);
  gegl_path_get_bounds (vector, &xmin, &xmax, &ymin, &ymax);
  extent.x = floor (xmin);
  extent.y = floor (ymin);
  extent.width = ceil (xmax) - extent.x;
  extent.height = ceil (ymax) - extent.y;

  if (!gegl_rectangle_intersect (&extent, &extent, clip_rect))
   {
     return;
   }

  while (iter)
    {
      /*fprintf (stderr, "%c, %i %i\n", iter->d.type, iter->d.point[0].x, iter->d.point[0].y);*/
      switch (iter->d.type)
        {
          case 'M':
            x = iter->d.point[0].x;
            y = iter->d.point[0].y;
            need_to_travel = 0;
            traveled_length = 0;
            break;
          case 'L':
            {
              GeglPathPoint a,b;

              gfloat spacing;
              gfloat local_pos;
              gfloat distance;
              gfloat offset;
              gfloat leftover;
              gfloat radius = linewidth / 2.0;


              a.x = x;
              a.y = y;

              b.x = iter->d.point[0].x;
              b.y = iter->d.point[0].y;

              spacing = 0.2 * radius;

              distance = gegl_path_point_dist (&a, &b);

              leftover = need_to_travel - traveled_length;
              offset = spacing - leftover;

              local_pos = offset;

              if (distance > 0)
                for (;
                     local_pos <= distance;
                     local_pos += spacing)
                  {
                    GeglPathPoint spot;
                    gfloat ratio = local_pos / distance;
                    gfloat radius = linewidth/2;

                    gegl_path_point_lerp (&spot, &a, &b, ratio);

                    gegl_path_stamp (buffer, clip_rect,
                      spot.x, spot.y, radius, hardness, color, opacity);

                    traveled_length += spacing;
                  }

              need_to_travel += distance;

              x = b.x;
              y = b.y;
            }

            break;
          case 'u':
            g_error ("stroking uninitialized path\n");
            break;
          case 's':
            break;
          default:
            g_error ("can't stroke for instruction: %i\n", iter->d.type);
            break;
        }
      iter=iter->next;
    }
}

static void
gegl_path_stamp (GeglBuffer *buffer,
                 const GeglRectangle *clip_rect,
                 gdouble     x,
                 gdouble     y,
                 gdouble     radius,
                 gdouble     hardness,
                 GeglColor  *color,
                 gdouble     opacity)
{
  gfloat col[4];
  StampStatic s = {FALSE,}; /* there should be a cache of stamps,
                               note that stamps are accessed in multiple threads
                             */

  GeglRectangle temp;
  GeglRectangle roi;

  roi.x = floor(x-radius);
  roi.y = floor(y-radius);
  roi.width = ceil (x+radius) - floor (x-radius);
  roi.height = ceil (y+radius) - floor (y-radius);

  gegl_color_get_pixel (color, babl_format ("RGBA float"), col);

  /* bail out if we wouldn't leave a mark on the buffer */
  if (!gegl_rectangle_intersect (&temp, &roi, clip_rect))
    {
      return;
    }

  if (s.format == NULL)
    s.format = babl_format ("RaGaBaA float");

  if (s.buf == NULL ||
      s.radius != radius)
    {
      if (s.buf != NULL)
        g_free (s.buf);
      /* allocate a little bit more, just in case due to rounding errors and
       * such */
      s.buf = g_malloc (4*4* (roi.width + 2 ) * (roi.height + 2));
      s.radius = radius;
      s.valid = TRUE;
    }
  g_assert (s.buf);

  gegl_buffer_get (buffer, &roi, 1.0, s.format, s.buf, 0, GEGL_ABYSS_NONE);

  {
    gint u, v;
    gint i=0;

    gfloat radius_squared = radius * radius;
    gfloat inner_radius_squared = (radius * hardness)*(radius * hardness);
    gfloat soft_range = radius_squared - inner_radius_squared;

    for (v= roi.y; v < roi.y + roi.height ; v++)
    {
      gfloat vy2 = (v-y)*(v-y);
      for (u= roi.x; u < roi.x + roi.width; u++)
        {
          gfloat o = (u-x) * (u-x) + vy2;

          if (o < inner_radius_squared)
             o = col[3];
          else if (o < radius_squared)
            {
              o = (1.0 - (o-inner_radius_squared) / (soft_range)) * col[3];
            }
          else
            {
              o=0.0;
            }
         if (o!=0.0)
           {
             gint c;
             o = o*opacity;
             for (c=0;c<4;c++)
               s.buf[i*4+c] = (s.buf[i*4+c] * (1.0-o) + col[c] * o);
           }
         i++;
        }
    }
  }
  gegl_buffer_set (buffer, &roi, 0, s.format, s.buf, 0);
  g_free (s.buf);
}

static void path_changed (GeglPath *path,
                          const GeglRectangle *roi,
                          gpointer userdata)
{
  GeglRectangle rect = *roi;
  GeglProperties    *o   = GEGL_PROPERTIES (userdata);
  /* invalidate the incoming rectangle */

  rect.x -= o->stroke_width/2;
  rect.y -= o->stroke_width/2;
  rect.width += o->stroke_width;
  rect.height += o->stroke_width;

  gegl_operation_invalidate (userdata, &rect, FALSE);
};

static void
prepare (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  gegl_operation_set_format (operation, "output", babl_format ("R'aG'aB'aA float"));
  if (o->transform && o->transform[0] != '\0')
    {
      GeglMatrix3 matrix;
      gegl_matrix3_parse_string (&matrix, o->transform);
      gegl_path_set_matrix (o->d, &matrix);
    }
}

static GeglRectangle
get_bounding_box (GeglOperation *operation)
{
  GeglProperties    *o       = GEGL_PROPERTIES (operation);
  GeglRectangle  defined = { 0, 0, 512, 512 };
  GeglRectangle *in_rect;
  gdouble        x0, x1, y0, y1;

  in_rect =  gegl_operation_source_get_bounding_box (operation, "input");

  gegl_path_get_bounds (o->d, &x0, &x1, &y0, &y1);
  defined.x      = x0 - o->stroke_width/2;
  defined.y      = y0 - o->stroke_width/2;
  defined.width  = x1 - x0 + o->stroke_width;
  defined.height = y1 - y0 + o->stroke_width;

  if (in_rect)
    {
      gegl_rectangle_bounding_box (&defined, &defined, in_rect);
    }

  return defined;
}

static void gegl_path_cairo_play (GeglPath *path,
                                    cairo_t *cr);

static gboolean
process (GeglOperation       *operation,
         GeglBuffer          *input,
         GeglBuffer          *output,
         const GeglRectangle *result,
         gint                 level)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);

  if (input)
    {
      gegl_buffer_copy (input, result, GEGL_ABYSS_NONE,
                        output, result);
    }
  else
    {
      gegl_buffer_clear (output, result);
    }

  if (o->fill_opacity > 0.0001 && o->fill)
    {
      gdouble color[4] = {0, 0, 0, 0};
      gegl_color_get_pixel (o->fill, babl_format ("R'G'B'A double"), color);
      color[3] *= o->fill_opacity;

      if (color[3] > 0.001)
        {
          static GMutex mutex = { 0, };
          cairo_t *cr;
          cairo_surface_t *surface;
          guchar *data;

          g_mutex_lock (&mutex);
          data = gegl_buffer_linear_open (output, result, NULL, babl_format ("B'aG'aR'aA u8"));
          surface = cairo_image_surface_create_for_data (data,
                                                         CAIRO_FORMAT_ARGB32,
                                                         result->width,
                                                         result->height,
                                                         result->width * 4);
          cr = cairo_create (surface);
          cairo_translate (cr, -result->x, -result->y);

          if (g_str_equal (o->fill_rule, "evenodd"))
            {
              cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
            }

          gegl_path_cairo_play (o->d, cr);
          cairo_set_source_rgba (cr, color[0], color[1], color[2], color[3]);
          cairo_fill (cr);

          g_mutex_unlock (&mutex);
          gegl_buffer_linear_close (output, data);
        }
    }

  g_object_set_data (G_OBJECT (operation), "path-radius", GINT_TO_POINTER((gint)(o->stroke_width+1)/2));

  if (o->stroke_width > 0.1 && o->stroke_opacity > 0.0001)
    {
      gegl_path_stroke (output, result,
                                o->d,
                                o->stroke,
                                o->stroke_width,
                                o->stroke_hardness,
                                o->stroke_opacity);
    }

  return  TRUE;
}


static void foreach_cairo (const GeglPathItem *knot,
                           gpointer              cr)
{
  switch (knot->type)
    {
      case 'M':
        cairo_move_to (cr, knot->point[0].x, knot->point[0].y);
        break;
      case 'L':
        cairo_line_to (cr, knot->point[0].x, knot->point[0].y);
        break;
      case 'C':
        cairo_curve_to (cr, knot->point[0].x, knot->point[0].y,
                            knot->point[1].x, knot->point[1].y,
                            knot->point[2].x, knot->point[2].y);
        break;
      case 'z':
        cairo_close_path (cr);
        break;
      default:
        g_print ("%s uh?:%c\n", G_STRLOC, knot->type);
    }
}

static void gegl_path_cairo_play (GeglPath *path,
                                    cairo_t *cr)
{
  gegl_path_foreach_flat (path, foreach_cairo, cr);
}

static GeglNode *detect (GeglOperation *operation,
                         gint           x,
                         gint           y)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  cairo_t *cr;
  cairo_surface_t *surface;
  gchar *data = "     ";
  gboolean result = FALSE;

  surface = cairo_image_surface_create_for_data ((guchar*)data,
                                                 CAIRO_FORMAT_ARGB32,
                                                 1,1,4);
  cr = cairo_create (surface);
  gegl_path_cairo_play (o->d, cr);
  cairo_set_line_width (cr, o->stroke_width);


  if (o->stroke_width > 0.1 && o->stroke_opacity > 0.0001)
    result = cairo_in_stroke (cr, x, y);

  if (!result)
    {
      if (o->d)
        {
          gdouble r,g,b,a;
          gegl_color_get_rgba (o->fill, &r,&g,&b,&a);
          if (a * o->fill_opacity>0.8)
            result = cairo_in_fill (cr, x, y);
        }
    }


  cairo_destroy (cr);

  if (result)
    return operation->node;

  return NULL;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass       *operation_class;
  GeglOperationFilterClass *filter_class;
  const gchar *composition =
    "<gegl>"
    "<node operation='gegl:crop' width='200' height='200'/>"
    "<node operation='gegl:over'>"
    "<node operation='gegl:translate' x='40' y='40'/>"
    "<node operation='gegl:path'>"
    "  <params>"
    "    <param name='fill'>rgba(1,1,1,0.2)</param>"
    "    <param name='stroke'>rgba(0,0,1,0.5)</param>"
    "    <param name='stroke-hardness'>0.0</param>"
    "    <param name='stroke-width'>8.0</param>"
    "    <param name='d'>"
    "M0,50 C0,78 24,100 50,100 C77,100 100,78 100,50 C100,45 99,40 98,35 C82,35 66,35 50,35 C42,35 35,42 35,50 C35,58 42,65 50,65 C56,65 61,61 64,56 C67,51 75,55 73,60 C69,  69 60,75 50,75 C36,75 25,64 25,50 C25,36 36,25 50,25 L93,25 C83,9 67,0 49,0 C25,0 0,20 0,50   z"
    "                    </param>"
    "  </params>"
    "</node>"
    "</node>"
    "<node operation='gegl:checkerboard' color1='rgb(0.25,0.25,0.25)' color2='rgb(0.75,0.75,0.75)'/>"
    "</gegl>";

  operation_class = GEGL_OPERATION_CLASS (klass);
  filter_class    = GEGL_OPERATION_FILTER_CLASS (klass);

  filter_class->process = process;
  operation_class->get_bounding_box = get_bounding_box;
  operation_class->prepare = prepare;
  operation_class->detect = detect;
  /*operation_class->no_cache = TRUE;*/

  gegl_operation_class_set_keys (operation_class,
    "name",         "gegl:path",
    "title",        _("Render Path"),
    "categories",   "render",
    "reference-composition", composition,
    "description" , _("Renders a brush stroke"),
    NULL);
}


#endif
