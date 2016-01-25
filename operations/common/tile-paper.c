/* This file is an image processing operation for GEGL
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
 * Contains code originaly from GIMP tile-paper.c, copyright
 * Copyright 1997-1999 Hirotsuna Mizuno <s1041150@u-aizu.ac.jp>
 *
 * Tile paper ported to GEGL:
 * Copyright 2015 Akash Hiremath (akash akya) <akashh246@gmail.com>
 */

#include "config.h"
#include <glib/gi18n-lib.h>
#include <stdlib.h>


#ifdef GEGL_PROPERTIES

enum_start (gegl_tile_paper_background_type)
  enum_value (GEGL_BACKGROUND_TYPE_TRANSPARENT, "transparent", N_("Transparent"))
  enum_value (GEGL_BACKGROUND_TYPE_INVERT,      "invert",      N_("Inverted image"))
  enum_value (GEGL_BACKGROUND_TYPE_IMAGE,       "image",       N_("Image"))
  enum_value (GEGL_BACKGROUND_TYPE_COLOR,       "color",       N_("Color"))
enum_end (GeglTilePaperBackgroundType)

enum_start (gegl_tile_paper_fractional_type)
  enum_value (GEGL_FRACTIONAL_TYPE_BACKGROUND, "background", N_("Background"))
  enum_value (GEGL_FRACTIONAL_TYPE_IGNORE,     "ignore",     N_("Ignore"))
  enum_value (GEGL_FRACTIONAL_TYPE_FORCE,      "force",      N_("Force"))
enum_end (GeglTilePaperFractionalType)

property_int (tile_width, _("Tile Width"), 155)
  description (_("Width of the tile"))
  value_range (1, G_MAXINT)
  ui_range    (1, 1500)
  ui_meta     ("unit", "pixel-distance")
  ui_meta     ("axis", "x")

property_int (tile_height, _("Tile Height"), 56)
  description (_("Height of the tile"))
  value_range (1, G_MAXINT)
  ui_range    (1, 1500)
  ui_meta     ("unit", "pixel-distance")
  ui_meta     ("axis", "y")

property_double (move_rate, _("Move rate"), 25.0)
  description (_("Move rate"))
  value_range (1.0, 100.0)
  ui_range    (1.0, 100.0)
  ui_meta     ("unit", "percent")

property_boolean (wrap_around, _("Wrap around"), FALSE)
  description (_("Wrap the fractional tiles"))

property_enum (fractional_type, _("Fractional type"),
               GeglTilePaperFractionalType, gegl_tile_paper_fractional_type,
               GEGL_FRACTIONAL_TYPE_FORCE)
  description (_("Fractional Type"))

property_boolean (centering, _("Centering"), TRUE)
  description (_("Centering of the tiles"))

property_enum (background_type, _("Background type"),
               GeglTilePaperBackgroundType, gegl_tile_paper_background_type,
               GEGL_BACKGROUND_TYPE_INVERT)
  description (_("Background type"))

property_color (bg_color, _("Background color"), "rgba(0.0, 0.0, 0.0, 1.0)")
  description (("The tiles' background color"))
  ui_meta     ("role", "color-primary")

#else

#define GEGL_OP_AREA_FILTER
#define GEGL_OP_C_SOURCE tile-paper.c

#include "gegl-op.h"
#include <math.h>
#include <stdio.h>


typedef struct _Tile
{
  guint x;
  guint y;
  gint  z;
  guint width;
  guint height;
  gint  move_x;
  gint  move_y;
} Tile;


static gint
tile_compare (const void *x,
              const void *y)
{
  return ((Tile *) x)->z - ((Tile *) y)->z;
}

static inline void
random_move (gint  *x,
             gint  *y,
             gint   max,
             GRand *gr)
{
  gdouble angle  = g_rand_double_range (gr, 0.0, 1.0) * G_PI;
  gdouble radius = g_rand_double_range (gr, 0.0, 1.0) * (gdouble) max;

  *x = (gint) (radius * cos (angle));
  *y = (gint) (radius * sin (angle));
}

static void
prepare (GeglOperation *operation)
{
  GeglOperationAreaFilter *op_area = GEGL_OPERATION_AREA_FILTER (operation);
  GeglProperties          *o       = GEGL_PROPERTIES (operation);

  op_area->left   = o->tile_width;
  op_area->right  = o->tile_width;
  op_area->top    = o->tile_height;
  op_area->bottom = o->tile_height;

  gegl_operation_set_format (operation, "input",  babl_format ("RGBA float"));
  gegl_operation_set_format (operation, "output", babl_format ("RGBA float"));
}

static void
randomize_tiles (GeglProperties      *o,
                 const GeglRectangle *rect,
                 gint                 division_x,
                 gint                 division_y,
                 gint                 offset_x,
                 gint                 offset_y,
                 gint                 n_tiles,
                 Tile                *tiles)
{
  Tile  *t = tiles;
  gint   move_max_pixels = o->move_rate * o->tile_width / 100;
  gint   x;
  gint   y;
  GRand *gr;

  gr = g_rand_new ();

  for (y = 0; y < division_y; y++)
    {
      gint srcy = offset_y + o->tile_height * y;

      for (x = 0; x < division_x; x++, t++)
        {
          gint srcx = offset_x + o->tile_width * x;

          if (srcx < 0)
            {
              t->x     = 0;
              t->width = srcx + o->tile_width;
            }
          else if (srcx + o->tile_width < rect->width)
            {
              t->x     = srcx;
              t->width = o->tile_width;
            }
          else
            {
              t->x     = srcx;
              t->width = rect->width - srcx;
            }

          if (srcy < 0)
            {
              t->y      = 0;
              t->height = srcy + o->tile_height;
            }
          else if (srcy + o->tile_height < rect->height)
            {
              t->y      = srcy;
              t->height = o->tile_height;
            }
          else
            {
              t->y      = srcy;
              t->height = rect->height - srcy;
            }

          t->z = g_rand_int (gr);
          random_move (&t->move_x, &t->move_y, move_max_pixels, gr);
        }
    }

  qsort (tiles, n_tiles, sizeof (*tiles), tile_compare);
}

static void
draw_tiles (GeglProperties      *o,
            const GeglRectangle *rect,
            GeglBuffer          *input,
            GeglBuffer          *output,
            gint                 num_of_tiles,
            Tile                *tiles)
{
  const Babl *format;
  gfloat     *tile_buffer;
  Tile       *t;
  gint        i;

  format = babl_format ("RGBA float");
  tile_buffer = g_new0 (gfloat, 4 * o->tile_width * o->tile_height);

  if (o->wrap_around)
    {
      for (t = tiles, i = 0; i < num_of_tiles; i++, t++)
        {
          GeglRectangle tile_rect = { t->x, t->y, t->width, t->height };

          gegl_buffer_get (input, &tile_rect, 1.0, format, tile_buffer,
                           GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);

          tile_rect.x += t->move_x;
          tile_rect.y += t->move_y;

          gegl_buffer_set (output, &tile_rect, 0, format,
                           tile_buffer, GEGL_AUTO_ROWSTRIDE);

          if (tile_rect.x < 0 || tile_rect.x + tile_rect.width > rect->width ||
              tile_rect.y < 0 || tile_rect.y + tile_rect.height > rect->height)
            {
              if (tile_rect.x < 0)
                {
                  tile_rect.x = rect->width + tile_rect.x;
                }
              else if (tile_rect.x+tile_rect.width > rect->width)
                {
                  tile_rect.x -= rect->width;
                }

              if (tile_rect.y < 0)
                {
                  tile_rect.y = rect->height + tile_rect.y;
                }
              else if (tile_rect.y + tile_rect.height > rect->height)
                {
                  tile_rect.y -= rect->height;
                }

              gegl_buffer_set (output, &tile_rect, 0, format,
                               tile_buffer, GEGL_AUTO_ROWSTRIDE);
            }
        }
    }
  else
    {
      for (t = tiles, i = 0; i < num_of_tiles; i++, t++)
        {
          GeglRectangle tile_rect = { t->x, t->y, t->width, t->height };

          gegl_buffer_get (input, &tile_rect, 1.0, format, tile_buffer,
                           GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);

          tile_rect.x += t->move_x;
          tile_rect.y += t->move_y;

          gegl_buffer_set (output, &tile_rect, 0, format,
                           tile_buffer, GEGL_AUTO_ROWSTRIDE);
        }
    }

  g_free (tile_buffer);
}

static void
set_background (GeglProperties      *o,
                const GeglRectangle *rect,
                GeglBuffer          *input,
                GeglBuffer          *output,
                gint                 division_x,
                gint                 division_y,
                gint                 offset_x,
                gint                 offset_y)
{
  const Babl *format;
  gfloat     *dest_buf;
  gfloat     *buf;
  gint        x;
  gint        y;
  gint        index;
  gint        clear_x0;
  gint        clear_y0;
  gint        clear_x1;
  gint        clear_y1;

  format = babl_format ("RGBA float");
  dest_buf = g_new0 (gfloat, 4 * rect->width * rect->height);

  if (o->fractional_type == GEGL_FRACTIONAL_TYPE_IGNORE)
    {
      clear_x0     = offset_x;
      clear_y0     = offset_y;
      clear_x1     = clear_x0 + o->tile_width * (rect->width / o->tile_width);
      clear_y1     = clear_y0 + o->tile_height * (rect->height / o->tile_height);
    }
  else
    {
      clear_x0     = 0;
      clear_y0     = 0;
      clear_x1     = clear_x0 + rect->width;
      clear_y1     = clear_y0 + rect->height;
    }

  switch (o->background_type)
    {
    case GEGL_BACKGROUND_TYPE_TRANSPARENT:
      gegl_buffer_set_color (output, rect,
                             gegl_color_new ("rgba(0.0,0.0,0.0,0.0)"));
      break;

    case GEGL_BACKGROUND_TYPE_COLOR:
      gegl_buffer_set_color (output, rect, o->bg_color);
      break;

    case GEGL_BACKGROUND_TYPE_INVERT:
      gegl_buffer_get (input, rect, 1.0, format, dest_buf,
                       GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
      buf = dest_buf;
      for (y = clear_y0; y < clear_y1; y++)
        {
          for (x = clear_x0; x < clear_x1; x++)
            {
              index         = 4 * (y * rect->width + x);
              buf[index]    = 1 - buf[index];
              buf[index+1]  = 1 - buf[index+1];
              buf[index+2]  = 1 - buf[index+2];
            }
        }
      gegl_buffer_set (output, rect, 0, format, dest_buf, GEGL_AUTO_ROWSTRIDE);
      break;

    case GEGL_BACKGROUND_TYPE_IMAGE:
      gegl_buffer_get (input, rect, 1.0, format, dest_buf,
                       GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
      gegl_buffer_set (output, rect, 0, format, dest_buf, GEGL_AUTO_ROWSTRIDE);
      break;
    }

  g_free (dest_buf);
}

static gboolean
process (GeglOperation       *operation,
         GeglBuffer          *input,
         GeglBuffer          *output,
         const GeglRectangle *result,
         gint                 level)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  Tile           *tiles;
  gint            offset_x;
  gint            offset_y;
  gint            division_x;
  gint            division_y;
  gint            n_tiles;

  division_x = result->width / o->tile_width;
  division_y = result->height / o->tile_height;

  offset_x = 0;
  offset_y = 0;

  if (o->fractional_type == GEGL_FRACTIONAL_TYPE_FORCE)
    {
      if (0 < result->width  % o->tile_width) division_x++;
      if (0 < result->height % o->tile_height) division_y++;

      if (o->centering)
        {
          if (1 < result->width % o->tile_width)
            {
              division_x++;
              offset_x = (result->width % o->tile_width) / 2 - o->tile_width;
            }

          if (1 < result->height % o->tile_height)
            {
              division_y++;
              offset_y = (result->height % o->tile_height) / 2 - o->tile_height;
            }
        }
    }
  else
    {
      if (o->centering)
        {
          offset_x = (result->width  % o->tile_width) / 2;
          offset_y = (result->height % o->tile_height) / 2;
        }
    }

  n_tiles = division_x * division_y;
  tiles = g_new(Tile, n_tiles);

  randomize_tiles (o, result,  division_x, division_y,
                   offset_x, offset_y, n_tiles, tiles);
  set_background  (o, result, input, output, division_x, division_y,
                   offset_x, offset_y);

  draw_tiles (o, result, input, output, n_tiles, tiles);

  return  TRUE;
}

static GeglRectangle
get_bounding_box (GeglOperation *operation)
{
  GeglRectangle  result  = { 0, 0, 0, 0 };
  GeglRectangle *in_rect = gegl_operation_source_get_bounding_box (operation,
                                                                   "input");

  if (! in_rect)
    return result;

  gegl_rectangle_copy (&result, in_rect);

  return result;
}

/* Compute the input rectangle required to compute the specified
 * region of interest (roi).
 */
static GeglRectangle
get_required_for_output (GeglOperation       *operation,
                         const gchar         *input_pad,
                         const GeglRectangle *roi)
{
  GeglRectangle  result = get_bounding_box (operation);
  return result;
}

static GeglRectangle
get_cached_region (GeglOperation       *operation,
                   const GeglRectangle *roi)
{
  return *gegl_operation_source_get_bounding_box (operation, "input");
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass       *operation_class;
  GeglOperationFilterClass *filter_class;

  operation_class = GEGL_OPERATION_CLASS (klass);
  filter_class    = GEGL_OPERATION_FILTER_CLASS (klass);

  operation_class->threaded                = FALSE;
  operation_class->prepare                 = prepare;
  operation_class->get_bounding_box        = get_bounding_box;
  operation_class->get_required_for_output = get_required_for_output;
  operation_class->get_cached_region       = get_cached_region;

  filter_class->process                    = process;

  gegl_operation_class_set_keys (operation_class,
    "name",               "gegl:tile-paper",
    "title",               _("Paper Tile"),
    "categories",         "artistic:map",
    "license",            "GPL3+",
    "position-dependent", "true",
    "description",        _("Cut image into paper tiles, and slide them"),
    NULL);
}

#endif
