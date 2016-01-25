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
 * Copyright 2009 Martin Nordholts
 */

#ifndef __GEGL_DOT_VISITOR_H__
#define __GEGL_DOT_VISITOR_H__

#include "graph/gegl-visitor.h"

G_BEGIN_DECLS


#define GEGL_TYPE_DOT_VISITOR            (gegl_dot_visitor_get_type ())
#define GEGL_DOT_VISITOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEGL_TYPE_DOT_VISITOR, GeglDotVisitor))
#define GEGL_DOT_VISITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GEGL_TYPE_DOT_VISITOR, GeglDotVisitorClass))
#define GEGL_IS_DOT_VISITOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEGL_TYPE_DOT_VISITOR))
#define GEGL_IS_DOT_VISITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GEGL_TYPE_DOT_VISITOR))
#define GEGL_DOT_VISITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GEGL_TYPE_DOT_VISITOR, GeglDotVisitorClass))

typedef struct _GeglDotVisitorPriv GeglDotVisitorPriv;
typedef struct _GeglDotVisitorClass GeglDotVisitorClass;

struct _GeglDotVisitor
{
  GeglVisitor         parent_instance;

  GeglDotVisitorPriv *priv;
};

struct _GeglDotVisitorClass
{
  GeglVisitorClass  parent_class;
};


GType   gegl_dot_visitor_get_type             (void) G_GNUC_CONST;

void    gegl_dot_visitor_set_string_to_append (GeglDotVisitor *dot_visitor,
                                               GString        *string_to_append);


G_END_DECLS

#endif /* __GEGL_DOT_VISITOR_H__ */
