/* This file is part of GEGL
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
 * Copyright 2006 Philip Lafleur
 */

#include "config.h"
#include <gegl-plugin.h>
#include "module.h"
#include "transform-core.h"

static GTypeModule          *transform_module;
static const GeglModuleInfo  modinfo =
{
  GEGL_MODULE_ABI_VERSION
};

G_MODULE_EXPORT GTypeModule *
transform_module_get_module (void)
{
  return transform_module;
}

G_MODULE_EXPORT const GeglModuleInfo *
gegl_module_query (GTypeModule *module)
{
  return &modinfo;
}

GType rotate_get_type      (void);
GType rotate_on_center_get_type (void);
GType scale_ratio_get_type  (void);
GType scale_size_get_type   (void);
GType scale_size_keepaspect_get_type   (void);
GType shear_get_type       (void);
GType translate_get_type   (void);
GType reflect_get_type     (void);
GType transform_get_type   (void);

G_MODULE_EXPORT gboolean
gegl_module_register (GTypeModule *module)
{
  GType dummy;
  transform_module = module;

  dummy = op_transform_get_type ();
  dummy = rotate_get_type ();
  dummy = rotate_on_center_get_type ();
  dummy = scale_ratio_get_type ();
  dummy = scale_size_get_type ();
  dummy = scale_size_keepaspect_get_type ();
  dummy = shear_get_type ();
  dummy = translate_get_type ();
  dummy = reflect_get_type ();
  dummy = transform_get_type ();

  dummy ++; /* silence gcc, having it is required to avoid optimizing
               away the _get_type calls themselves */

  return TRUE;
}
