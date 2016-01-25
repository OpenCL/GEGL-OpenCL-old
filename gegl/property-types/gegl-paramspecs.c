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
 * Copyright 2003 Calvin Williamson
 *           2006 Øyvind Kolås
 *
 * Original contents copied from gimp/app/core/gimpparamspecs.h
 * (c) 1995-2006 Spencer Kimball, Peter Mattis and others.
 */

#include "config.h"

#include <glib.h>
#include <glib-object.h>
#include "gegl-paramspecs.h"
#include "gegl-types.h"
#include <babl/babl.h>
#include "gegl-color.h"
#include "gegl-audio-fragment.h"
#include "gegl-curve.h"
#include "gegl-path.h"

static void       gegl_param_double_class_init (GParamSpecClass *klass);
static void       gegl_param_double_init       (GParamSpec      *pspec);

GType
gegl_param_double_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gegl_param_double_class_init,
        NULL, NULL,
        sizeof (GeglParamSpecDouble),
        0,
        (GInstanceInitFunc) gegl_param_double_init
      };
      type = g_type_register_static (G_TYPE_PARAM_DOUBLE,
                                     "GeglParamDouble", &info, 0);
    }
  return type;
}

static void
gegl_param_double_class_init (GParamSpecClass *klass)
{
  klass->value_type     = G_TYPE_DOUBLE;
}

static void
gegl_param_double_init (GParamSpec *pspec)
{
  GParamSpecDouble    *dpspec  = G_PARAM_SPEC_DOUBLE (pspec);
  GeglParamSpecDouble *gdpspec = GEGL_PARAM_SPEC_DOUBLE (pspec);
  gdpspec->ui_minimum = dpspec->minimum;
  gdpspec->ui_maximum = dpspec->maximum;
  gdpspec->ui_gamma   = 1.0;
}

GParamSpec *
gegl_param_spec_double (const gchar *name,
                        const gchar *nick,
                        const gchar *blurb,
                        gdouble      minimum,
                        gdouble      maximum,
                        gdouble      default_value,
                        gdouble      ui_minimum,
                        gdouble      ui_maximum,
                        gdouble      ui_gamma,
                        GParamFlags  flags)
{
  GeglParamSpecDouble *pspec;
  GParamSpecDouble *dspec;

  pspec = g_param_spec_internal (GEGL_TYPE_PARAM_DOUBLE,
                                 name, nick, blurb, flags);
  dspec = G_PARAM_SPEC_DOUBLE (pspec);

  dspec->minimum = minimum;
  dspec->maximum = maximum;
  dspec->default_value = default_value;
  pspec->ui_minimum = ui_minimum;
  pspec->ui_maximum = ui_maximum;
  pspec->ui_gamma   = ui_gamma;
  gegl_param_spec_double_set_steps (pspec, 0.1, 1.0);
  gegl_param_spec_double_set_digits (pspec, 2);

  return G_PARAM_SPEC (pspec);
}

void
gegl_param_spec_double_set_steps (GeglParamSpecDouble *pspec,
                                  gdouble              step_small,
                                  gdouble              step_big)
{
  g_return_if_fail (GEGL_IS_PARAM_SPEC_DOUBLE (pspec));

  pspec->ui_step_small = step_small;
  pspec->ui_step_big = step_big;
}

void gegl_param_spec_double_set_digits (GeglParamSpecDouble *pspec,
                                        gint                 digits)
{
  g_return_if_fail (GEGL_IS_PARAM_SPEC_DOUBLE (pspec));

  pspec->ui_digits = digits;
}

gdouble gegl_param_spec_double_get_step_size (GeglParamSpecDouble *pspec);
gdouble gegl_param_spec_double_get_page_size (GeglParamSpecDouble *pspec);

static void       gegl_param_int_class_init (GParamSpecClass *klass);
static void       gegl_param_int_init       (GParamSpec      *pspec);

GType
gegl_param_int_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gegl_param_int_class_init,
        NULL, NULL,
        sizeof (GeglParamSpecInt),
        0,
        (GInstanceInitFunc) gegl_param_int_init
      };
      type = g_type_register_static (G_TYPE_PARAM_INT,
                                     "GeglParamInt", &info, 0);
    }
  return type;
}

static void
gegl_param_int_class_init (GParamSpecClass *klass)
{
  klass->value_type     = G_TYPE_INT;
}

static void
gegl_param_int_init (GParamSpec *pspec)
{
  GParamSpecInt    *dpspec  = G_PARAM_SPEC_INT (pspec);
  GeglParamSpecInt *gdpspec = GEGL_PARAM_SPEC_INT (pspec);
  gdpspec->ui_minimum = dpspec->minimum;
  gdpspec->ui_maximum = dpspec->maximum;
  gdpspec->ui_gamma = 1.0;
}

GParamSpec *
gegl_param_spec_int (const gchar *name,
                     const gchar *nick,
                     const gchar *blurb,
                     gint         minimum,
                     gint         maximum,
                     gint         default_value,
                     gint         ui_minimum,
                     gint         ui_maximum,
                     gdouble      ui_gamma,
                     GParamFlags  flags)
{
  GeglParamSpecInt *pspec;
  GParamSpecInt *ispec;


  pspec = g_param_spec_internal (GEGL_TYPE_PARAM_INT,
                                 name, nick, blurb, flags);
  ispec = G_PARAM_SPEC_INT (pspec);

  ispec->minimum = minimum;
  ispec->maximum = maximum;
  ispec->default_value = default_value;
  pspec->ui_minimum = ui_minimum;
  pspec->ui_maximum = ui_maximum;
  pspec->ui_gamma = ui_gamma;

  gegl_param_spec_int_set_steps (pspec, 1, 5);
  return G_PARAM_SPEC (pspec);
}

void
gegl_param_spec_int_set_steps (GeglParamSpecInt *pspec,
                               gint              step_small,
                               gint              step_big)
{
  g_return_if_fail (GEGL_IS_PARAM_SPEC_INT (pspec));

  pspec->ui_step_small = step_small;
  pspec->ui_step_big = step_big;
}

/*
 * GEGL_TYPE_PARAM_STRING
 */

static void       gegl_param_string_class_init (GParamSpecClass *klass);
static void       gegl_param_string_init (GParamSpec *pspec);
static gboolean   gegl_param_string_validate (GParamSpec *pspec,
                                              GValue     *value);

GType
gegl_param_string_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL,                                          NULL,
        (GClassInitFunc) gegl_param_string_class_init,
        NULL,                                          NULL,
        sizeof (GeglParamSpecString),
        0,
        (GInstanceInitFunc) gegl_param_string_init
      };

      type = g_type_register_static (G_TYPE_PARAM_STRING,
                                     "GeglParamString", &info, 0);
    }

  return type;
}

static void
gegl_param_string_class_init (GParamSpecClass *klass)
{
  klass->value_type     = G_TYPE_STRING;
  klass->value_validate = gegl_param_string_validate;
}

static void
gegl_param_string_init (GParamSpec *pspec)
{
  GeglParamSpecString *sspec = GEGL_PARAM_SPEC_STRING (pspec);

  sspec->no_validate = FALSE;
  sspec->null_ok     = FALSE;
}

static gboolean
gegl_param_string_validate (GParamSpec *pspec,
                            GValue     *value)
{
  GeglParamSpecString *sspec  = GEGL_PARAM_SPEC_STRING (pspec);
  gchar               *string = value->data[0].v_pointer;

  if (string)
    {
      gchar *s;

      if (!sspec->no_validate &&
          !g_utf8_validate (string, -1, (const gchar **) &s))
        {
          for (; *s; s++)
            if (*s < ' ')
              *s = '?';

          return TRUE;
        }
    }
  else if (!sspec->null_ok)
    {
      value->data[0].v_pointer = g_strdup ("");
      return TRUE;
    }

  return FALSE;
}

GParamSpec *
gegl_param_spec_string (const gchar *name,
                        const gchar *nick,
                        const gchar *blurb,
                        gboolean     no_validate,
                        gboolean     null_ok,
                        const gchar *default_value,
                        GParamFlags  flags)
{
  GeglParamSpecString *sspec;

  sspec = g_param_spec_internal (GEGL_TYPE_PARAM_STRING,
                                 name, nick, blurb, flags);

  if (sspec)
    {
      g_free (G_PARAM_SPEC_STRING (sspec)->default_value);
      G_PARAM_SPEC_STRING (sspec)->default_value = g_strdup (default_value);

      sspec->no_validate = no_validate ? TRUE : FALSE;
      sspec->null_ok     = null_ok     ? TRUE : FALSE;
    }

  return G_PARAM_SPEC (sspec);
}


/*
 * GEGL_TYPE_PARAM_FILE_PATH
 */

static void       gegl_param_file_path_class_init (GParamSpecClass *klass);
static void       gegl_param_file_path_init       (GParamSpec *pspec);
static gboolean   gegl_param_file_path_validate   (GParamSpec *pspec,
                                                   GValue     *value);

GType
gegl_param_file_path_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL,                                        NULL,
        (GClassInitFunc) gegl_param_file_path_class_init,
        NULL,                                        NULL,
        sizeof (GeglParamSpecString),
        0,
        (GInstanceInitFunc) gegl_param_file_path_init
      };

      type = g_type_register_static (G_TYPE_PARAM_STRING,
                                     "GeglParamFilePath", &info, 0);
    }

  return type;
}

static void
gegl_param_file_path_class_init (GParamSpecClass *klass)
{
  klass->value_type     = G_TYPE_STRING;
  klass->value_validate = gegl_param_file_path_validate;
}

static void
gegl_param_file_path_init (GParamSpec *pspec)
{
  GeglParamSpecFilePath *sspec = GEGL_PARAM_SPEC_FILE_PATH (pspec);

  sspec->no_validate = FALSE;
  sspec->null_ok     = FALSE;
}

static gboolean
gegl_param_file_path_validate (GParamSpec *pspec,
                               GValue     *value)
{
  GeglParamSpecFilePath *sspec = GEGL_PARAM_SPEC_FILE_PATH (pspec);
  gchar                 *path  = value->data[0].v_pointer;

  if (path)
    {
      gchar *s;

      if (!sspec->no_validate &&
          !g_utf8_validate (path, -1, (const gchar **) &s))
        {
          for (; *s; s++)
            if (*s < ' ')
              *s = '?';

          return TRUE;
        }
    }
  else if (!sspec->null_ok)
    {
      value->data[0].v_pointer = g_strdup ("");
      return TRUE;
    }

  return FALSE;
}

GParamSpec *
gegl_param_spec_file_path (const gchar *name,
                           const gchar *nick,
                           const gchar *blurb,
                           gboolean     no_validate,
                           gboolean     null_ok,
                           const gchar *default_value,
                           GParamFlags  flags)
{
  GeglParamSpecFilePath *sspec;

  sspec = g_param_spec_internal (GEGL_TYPE_PARAM_FILE_PATH,
                                 name, nick, blurb, flags);

  if (sspec)
    {
      g_free (G_PARAM_SPEC_STRING (sspec)->default_value);
      G_PARAM_SPEC_STRING (sspec)->default_value = g_strdup (default_value);

      sspec->no_validate = no_validate ? TRUE : FALSE;
      sspec->null_ok     = null_ok     ? TRUE : FALSE;
    }

  return G_PARAM_SPEC (sspec);
}


/*
 * GEGL_TYPE_PARAM_MULTILINE
 */

static void       gegl_param_multiline_class_init (GParamSpecClass *klass);
static void       gegl_param_multiline_init (GParamSpec *pspec);
static gboolean   gegl_param_multiline_validate (GParamSpec *pspec,
                                                 GValue     *value);

GType
gegl_param_multiline_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL,                                             NULL,
        (GClassInitFunc) gegl_param_multiline_class_init,
        NULL,                                             NULL,
        sizeof (GeglParamSpecString),
        0,
        (GInstanceInitFunc) gegl_param_multiline_init
      };

      type = g_type_register_static (G_TYPE_PARAM_STRING,
                                     "GeglParamMultiline", &info, 0);
    }

  return type;
}

static void
gegl_param_multiline_class_init (GParamSpecClass *klass)
{
  klass->value_type     = G_TYPE_STRING;
  klass->value_validate = gegl_param_multiline_validate;
}

static void
gegl_param_multiline_init (GParamSpec *pspec)
{
  GeglParamSpecMultiline *sspec = GEGL_PARAM_SPEC_MULTILINE (pspec);

  sspec->no_validate = FALSE;
  sspec->null_ok     = FALSE;
}

static gboolean
gegl_param_multiline_validate (GParamSpec *pspec,
                               GValue     *value)
{
  GeglParamSpecMultiline *sspec     = GEGL_PARAM_SPEC_MULTILINE (pspec);
  gchar                  *multiline = value->data[0].v_pointer;

  if (multiline)
    {
      gchar *s;

      if (!sspec->no_validate &&
          !g_utf8_validate (multiline, -1, (const gchar **) &s))
        {
          for (; *s; s++)
            if (*s < ' ')
              *s = '?';

          return TRUE;
        }
    }
  else if (!sspec->null_ok)
    {
      value->data[0].v_pointer = g_strdup ("");
      return TRUE;
    }

  return FALSE;
}

GParamSpec *
gegl_param_spec_multiline (const gchar *name,
                           const gchar *nick,
                           const gchar *blurb,
                           const gchar *default_value,
                           GParamFlags  flags)
{
  GeglParamSpecMultiline *sspec;

  sspec = g_param_spec_internal (GEGL_TYPE_PARAM_MULTILINE,
                                 name, nick, blurb, flags);

  if (sspec)
    {
      g_free (G_PARAM_SPEC_STRING (sspec)->default_value);
      G_PARAM_SPEC_STRING (sspec)->default_value = g_strdup (default_value);
/*
      sspec->no_validate = no_validate ? TRUE : FALSE;
      sspec->null_ok     = null_ok     ? TRUE : FALSE;*/
    }

  return G_PARAM_SPEC (sspec);
}




/*
 * GEGL_TYPE_PARAM_ENUM
 */

static void       gegl_param_enum_class_init (GParamSpecClass *klass);
static void       gegl_param_enum_init       (GParamSpec      *pspec);
static void       gegl_param_enum_finalize   (GParamSpec      *pspec);
static gboolean   gegl_param_enum_validate   (GParamSpec      *pspec,
                                              GValue          *value);

GType
gegl_param_enum_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL,                                        NULL,
        (GClassInitFunc) gegl_param_enum_class_init,
        NULL,                                        NULL,
        sizeof (GeglParamSpecEnum),
        0,
        (GInstanceInitFunc) gegl_param_enum_init
      };

      type = g_type_register_static (G_TYPE_PARAM_ENUM,
                                     "GeglParamEnum", &info, 0);
    }

  return type;
}

static void
gegl_param_enum_class_init (GParamSpecClass *klass)
{
  klass->value_type     = G_TYPE_ENUM;
  klass->finalize       = gegl_param_enum_finalize;
  klass->value_validate = gegl_param_enum_validate;
}

static void
gegl_param_enum_init (GParamSpec *pspec)
{
  GeglParamSpecEnum *espec = GEGL_PARAM_SPEC_ENUM (pspec);

  espec->excluded_values = NULL;
}

static void
gegl_param_enum_finalize (GParamSpec *pspec)
{
  GeglParamSpecEnum *espec = GEGL_PARAM_SPEC_ENUM (pspec);
  GParamSpecClass   *parent_class;

  parent_class = g_type_class_peek (g_type_parent (GEGL_TYPE_PARAM_ENUM));

  g_slist_free (espec->excluded_values);

  parent_class->finalize (pspec);
}

static gboolean
gegl_param_enum_validate (GParamSpec *pspec,
                          GValue     *value)
{
  GeglParamSpecEnum *espec = GEGL_PARAM_SPEC_ENUM (pspec);
  GParamSpecClass   *parent_class;
  GSList            *list;

  parent_class = g_type_class_peek (g_type_parent (GEGL_TYPE_PARAM_ENUM));

  if (parent_class->value_validate (pspec, value))
    return TRUE;

  for (list = espec->excluded_values; list; list = g_slist_next (list))
    {
      if (GPOINTER_TO_INT (list->data) == value->data[0].v_long)
        {
          value->data[0].v_long = G_PARAM_SPEC_ENUM (pspec)->default_value;
          return TRUE;
        }
    }

  return FALSE;
}

GParamSpec *
gegl_param_spec_enum (const gchar *name,
                      const gchar *nick,
                      const gchar *blurb,
                      GType        enum_type,
                      gint         default_value,
                      GParamFlags  flags)
{
  GeglParamSpecEnum *espec;
  GEnumClass        *enum_class;

  g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

  enum_class = g_type_class_ref (enum_type);

  g_return_val_if_fail (g_enum_get_value (enum_class, default_value) != NULL,
                        NULL);

  espec = g_param_spec_internal (GEGL_TYPE_PARAM_ENUM,
                                 name, nick, blurb, flags);

  G_PARAM_SPEC_ENUM (espec)->enum_class    = enum_class;
  G_PARAM_SPEC_ENUM (espec)->default_value = default_value;
  G_PARAM_SPEC (espec)->value_type         = enum_type;

  return G_PARAM_SPEC (espec);
}

void
gegl_param_spec_enum_exclude_value (GeglParamSpecEnum *espec,
                                    gint               value)
{
  g_return_if_fail (GEGL_IS_PARAM_SPEC_ENUM (espec));
  g_return_if_fail (g_enum_get_value (G_PARAM_SPEC_ENUM (espec)->enum_class,
                                      value) != NULL);

  espec->excluded_values = g_slist_prepend (espec->excluded_values,
                                            GINT_TO_POINTER (value));
}

/*
 * GEGL_TYPE_PARAM_SEED
 */

static void       gegl_param_seed_class_init (GParamSpecClass *klass);
static void       gegl_param_seed_init       (GParamSpec      *pspec);

GType
gegl_param_seed_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gegl_param_seed_class_init,
        NULL, NULL,
        sizeof (GeglParamSpecSeed),
        0,
        (GInstanceInitFunc) gegl_param_seed_init
      };
      type = g_type_register_static (G_TYPE_PARAM_INT,
                                     "GeglParamSeed", &info, 0);
    }
  return type;
}

static void
gegl_param_seed_class_init (GParamSpecClass *klass)
{
  klass->value_type = G_TYPE_INT;
}

static void
gegl_param_seed_init (GParamSpec *pspec)
{
  GeglParamSpecSeed *gdpspec = GEGL_PARAM_SPEC_SEED (pspec);

  gdpspec->ui_minimum = 0;
  gdpspec->ui_maximum = G_MAXINT;
}

GParamSpec *
gegl_param_spec_seed (const gchar *name,
                      const gchar *nick,
                      const gchar *blurb,
                      GParamFlags  flags)
{
  GeglParamSpecSeed *pspec;
  GParamSpecInt     *ispec;

  pspec = g_param_spec_internal (GEGL_TYPE_PARAM_SEED,
                                 name, nick, blurb, flags);
  ispec = G_PARAM_SPEC_INT (pspec);

  ispec->minimum = 0;
  ispec->maximum = G_MAXINT;
  ispec->default_value = 0;
  pspec->ui_minimum = 0;
  pspec->ui_maximum = G_MAXINT;

  return G_PARAM_SPEC (pspec);
}

/*
 * GEGL_TYPE_PARAM_FORMAT
 */

static void       gegl_param_format_class_init (GParamSpecClass *klass);
static void       gegl_param_format_init       (GParamSpec      *pspec);

GType
gegl_param_format_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gegl_param_format_class_init,
        NULL, NULL,
        sizeof (GeglParamSpecFormat),
        0,
        (GInstanceInitFunc) gegl_param_format_init
      };
      type = g_type_register_static (G_TYPE_PARAM_POINTER,
                                     "GeglParamFormat", &info, 0);
    }
  return type;
}

static void
gegl_param_format_class_init (GParamSpecClass *klass)
{
  klass->value_type = G_TYPE_POINTER;
}

static void
gegl_param_format_init (GParamSpec *pspec)
{
}

GParamSpec *
gegl_param_spec_format (const gchar *name,
                        const gchar *nick,
                        const gchar *blurb,
                        GParamFlags  flags)
{
  GeglParamSpecFormat *pspec;

  pspec = g_param_spec_internal (GEGL_TYPE_PARAM_FORMAT,
                                 name, nick, blurb, flags);

  return G_PARAM_SPEC (pspec);
}

/*
 * GEGL_TYPE_PARAM_URI
 */

static void       gegl_param_uri_class_init (GParamSpecClass *klass);
static void       gegl_param_uri_init       (GParamSpec *pspec);
static gboolean   gegl_param_uri_validate   (GParamSpec *pspec,
                                                   GValue     *value);

GType
gegl_param_uri_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo info =
      {
        sizeof (GParamSpecClass),
        NULL,                                        NULL,
        (GClassInitFunc) gegl_param_uri_class_init,
        NULL,                                        NULL,
        sizeof (GeglParamSpecString),
        0,
        (GInstanceInitFunc) gegl_param_uri_init
      };

      type = g_type_register_static (G_TYPE_PARAM_STRING,
                                     "GeglParamUri", &info, 0);
    }

  return type;
}

static void
gegl_param_uri_class_init (GParamSpecClass *klass)
{
  klass->value_type     = G_TYPE_STRING;
  klass->value_validate = gegl_param_uri_validate;
}

static void
gegl_param_uri_init (GParamSpec *pspec)
{
  GeglParamSpecUri *sspec = GEGL_PARAM_SPEC_URI (pspec);

  sspec->no_validate = FALSE;
  sspec->null_ok     = FALSE;
}

static gboolean
gegl_param_uri_validate (GParamSpec *pspec,
                               GValue     *value)
{
  GeglParamSpecUri *sspec = GEGL_PARAM_SPEC_URI (pspec);
  gchar                 *path  = value->data[0].v_pointer;

  if (path)
    {
      gchar *s;

      if (!sspec->no_validate &&
          !g_utf8_validate (path, -1, (const gchar **) &s))
        {
          for (; *s; s++)
            if (*s < ' ')
              *s = '?';

          return TRUE;
        }
    }
  else if (!sspec->null_ok)
    {
      value->data[0].v_pointer = g_strdup ("");
      return TRUE;
    }

  return FALSE;
}

GParamSpec *
gegl_param_spec_uri (const gchar *name,
                           const gchar *nick,
                           const gchar *blurb,
                           gboolean     no_validate,
                           gboolean     null_ok,
                           const gchar *default_value,
                           GParamFlags  flags)
{
  GeglParamSpecUri *sspec;

  sspec = g_param_spec_internal (GEGL_TYPE_PARAM_URI,
                                 name, nick, blurb, flags);

  if (sspec)
    {
      g_free (G_PARAM_SPEC_STRING (sspec)->default_value);
      G_PARAM_SPEC_STRING (sspec)->default_value = g_strdup (default_value);

      sspec->no_validate = no_validate ? TRUE : FALSE;
      sspec->null_ok     = null_ok     ? TRUE : FALSE;
    }

  return G_PARAM_SPEC (sspec);
}

