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
 * Copyright 2006 Geert Jordaens <geert.jordaens@telenet.be>
 */


#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

property_string (values, _("Values"), "")
    description(_("list of <number>s"))

#else

#define GEGL_OP_POINT_FILTER
#define GEGL_OP_C_SOURCE svg-matrix.c

#include "gegl-op.h"
#include <math.h>
#include <stdlib.h>

static void prepare (GeglOperation *operation)
{
  const Babl *format = babl_format ("RaGaBaA float");

  gegl_operation_set_format (operation, "input", format);
  gegl_operation_set_format (operation, "output", format);
}

static gboolean
process (GeglOperation       *op,
         void                *in_buf,
         void                *out_buf,
         glong                n_pixels,
         const GeglRectangle *roi,
         gint                 level)
{
  GeglProperties *o = GEGL_PROPERTIES (op);
  gfloat         *in = in_buf;
  gfloat         *out = out_buf;
  gfloat         *m;

  gfloat mi[25] = { 1.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 1.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 1.0};
  gfloat ma[25] = { 1.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 1.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 1.0};
  char        *endptr;
  gfloat       value;
  const gchar  delimiter=',';
  const gchar *delimiters=" ";
  gchar      **values;
  glong        i;

  m = ma;

  if (o->values != NULL)
    {
      g_strstrip(o->values);
      g_strdelimit (o->values, delimiters, delimiter);
      values = g_strsplit (o->values, ",", 20);
      for (i = 0 ; i < 20 ; i++)
        if ( values[i] != NULL )
          {
            value = g_ascii_strtod(values[i], &endptr);
            if (endptr != values[i])
               ma[i] = value;
            else
              {
                m = mi;
                i = 21;
              }
          }
        else
          {
             m = mi;
             i = 21;
          }
       g_strfreev(values);
    }

  for (i=0; i<n_pixels; i++)
    {
      out[0] =  m[0]  * in[0] +  m[1]  * in[1] + m[2]  * in[2] + m[3]  * in[3] + m[4];
      out[1] =  m[5]  * in[0] +  m[6]  * in[1] + m[7]  * in[2] + m[8]  * in[3] + m[9];
      out[2] =  m[10] * in[0] +  m[11] * in[1] + m[12] * in[2] + m[13] * in[3] + m[14];
      out[3] =  m[15] * in[0] +  m[16] * in[1] + m[17] * in[2] + m[18] * in[3] + m[19];
      in  += 4;
      out += 4;
    }

  return TRUE;
}


static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass            *operation_class;
  GeglOperationPointFilterClass *point_filter_class;

  operation_class    = GEGL_OPERATION_CLASS (klass);
  point_filter_class = GEGL_OPERATION_POINT_FILTER_CLASS (klass);

  point_filter_class->process = process;
  operation_class->prepare = prepare;

  gegl_operation_class_set_keys (operation_class,
    "name"       , "gegl:svg-matrix",
    "categories" , "compositors:svgfilter",
    "description",_("SVG color matrix operation svg_matrix"),
    NULL);
}

#endif
