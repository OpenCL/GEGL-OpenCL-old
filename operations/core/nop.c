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

   /* no properties */

#else

#define GEGL_OP_FILTER
#define GEGL_OP_C_SOURCE        nop.c

#include "gegl-op.h"

static void
gegl_nop_prepare (GeglOperation *self)
{
  const Babl *fmt = gegl_operation_get_source_format (self, "input");

  gegl_operation_set_format (self, "output", fmt);
}

static gboolean
gegl_nop_process (GeglOperation        *operation,
                  GeglOperationContext *context,
                  const gchar          *output_prop,
                  const GeglRectangle  *result,
                  gint                  level)
{
  GeglBuffer *input;

  if (strcmp (output_prop, "output"))
    {
      g_warning ("requested processing of %s pad on a nop", output_prop);
      return FALSE;
    }

  input = GEGL_BUFFER (gegl_operation_context_get_object (context, "input"));
  if (!input)
    {
      g_warning ("nop received NULL input");
      return FALSE;
    }

  gegl_operation_context_take_object (context, "output", g_object_ref (G_OBJECT (input)));
  return TRUE;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);
  operation_class->process = gegl_nop_process;
  operation_class->prepare = gegl_nop_prepare;

  gegl_operation_class_set_keys (operation_class,
              "name",        "gegl:nop",
              "title",       _("No Operation"),
              "categories",  "core",
              "description", _("No operation (can be used as a routing point)"),
              NULL);
}

#endif
