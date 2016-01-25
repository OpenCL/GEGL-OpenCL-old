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

property_string (path, _("File"), "")
    description (_("a GeglBuffer on disk to open"))

#else

#define GEGL_OP_SOURCE
#define GEGL_OP_C_SOURCE open-buffer.c

#include "gegl-op.h"

static void buffer_changed (GeglBuffer          *buffer,
                            const GeglRectangle *rect,
                            gpointer             userdata)
{
  gegl_operation_invalidate (GEGL_OPERATION (userdata), rect, FALSE);
}

static GeglBuffer *ensure_buffer (GeglOperation *operation)
{
  GeglProperties   *o = GEGL_PROPERTIES (operation);
  GeglBuffer   *buffer = o->user_data;
  if (buffer)
    return buffer;
  if (!buffer)
    {
      buffer = gegl_buffer_open (o->path);
      o->user_data = buffer;
    }
  gegl_buffer_signal_connect (buffer, "changed",
                              G_CALLBACK(buffer_changed), operation);
  return buffer;
}

static GeglRectangle
get_bounding_box (GeglOperation *operation)
{
  GeglRectangle result = {0,0,0,0};
  GeglBuffer   *buffer = ensure_buffer (operation);

  result = *gegl_buffer_get_extent (GEGL_BUFFER (buffer));
  return result;
}


static GeglRectangle
get_cached_region (GeglOperation       *self,
                   const GeglRectangle *roi)
{
  return get_bounding_box (self);
}

static gboolean
process (GeglOperation         *operation,
         GeglOperationContext  *context,
         const gchar           *output_pad,
         const GeglRectangle   *result,
         gint                   level)
{
  GeglBuffer *buffer = ensure_buffer (operation);

  if (buffer)
    {
      g_object_ref (buffer); /* Add an extra reference, since
		              * gegl_operation_set_data is
		              * stealing one.
		              */

      /* override core behaviour, by resetting the buffer in the operation_context */
      gegl_operation_context_take_object (context, "output", G_OBJECT (buffer));
      return TRUE;
    }
  return FALSE;
}

static void
dispose (GObject *object)
{
  GeglProperties *o      = GEGL_PROPERTIES (object);
  GeglBuffer     *buffer = o->user_data;

  if (buffer)
    {
      g_object_unref (buffer);
      o->user_data = NULL;
    }

  G_OBJECT_CLASS (gegl_op_parent_class)->dispose (object);
}


static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass       *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  G_OBJECT_CLASS (klass)->dispose = dispose;

  operation_class->process = process;
  operation_class->get_bounding_box = get_bounding_box;
  operation_class->get_cached_region = get_cached_region;

  gegl_operation_class_set_keys (operation_class,
    "name"       , "gegl:open-buffer",
    "title",       _("Open GEGL Buffer"),
    "categories" , "input",
    "description", _("Use an on-disk GeglBuffer as data source."),
    NULL);

  operation_class->no_cache = TRUE;
}

#endif
