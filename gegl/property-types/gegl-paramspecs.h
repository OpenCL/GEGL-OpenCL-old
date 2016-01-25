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

#include <glib-object.h>

#ifndef __GEGL_PARAM_SPECS_H__
#define __GEGL_PARAM_SPECS_H__

G_BEGIN_DECLS

/*
 * Keep in sync with libgeglconfig/geglconfig-params.h
 */
#define GEGL_PARAM_NO_VALIDATE (1 << (6 + G_PARAM_USER_SHIFT))

typedef struct _GeglParamSpecString GeglParamSpecString;
typedef struct _GeglParamSpecDouble GeglParamSpecDouble;
typedef struct _GeglParamSpecInt    GeglParamSpecInt;
typedef struct _GeglParamSpecSeed   GeglParamSpecSeed;
typedef struct _GeglParamSpecFormat GeglParamSpecFormat;


/*
 * GEGL_TYPE_PARAM_DOUBLE
 */

#define GEGL_TYPE_PARAM_DOUBLE             (gegl_param_double_get_type ())
#define GEGL_PARAM_SPEC_DOUBLE(pspec)      (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_DOUBLE, GeglParamSpecDouble))
#define GEGL_IS_PARAM_SPEC_DOUBLE(pspec)   (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_DOUBLE))


struct _GeglParamSpecDouble
{
  GParamSpecDouble parent_instance;
  gdouble          ui_minimum; /* reasonable range to present to user */
  gdouble          ui_maximum;
  gdouble          ui_gamma;   /* a desired non-linear mapping or 1.0, useful
                                  when the control the user needs is not a
                                  linear mapping, like controlling brush-size
                                  or gaussian blur radius - where more
                                  detailed control of small values is needed
                                  */
  gdouble          ui_step_small;
  gdouble          ui_step_big;
  gint             ui_digits;
};

GType        gegl_param_double_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_double:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @ui_minimum: minimum value a user should be allowed to input
 * @ui_maximum: maximum value a user should be allowed to input
 * @ui_gamma: the gamma that should be used when adjusting the value
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecDouble instance.
 *
 * Return value: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_double (const gchar *name,
                                     const gchar *nick,
                                     const gchar *blurb,
                                     gdouble      minimum,
                                     gdouble      maximum,
                                     gdouble      default_value,
                                     gdouble      ui_minimum,
                                     gdouble      ui_maximum,
                                     gdouble      ui_gamma,
                                     GParamFlags  flags);


/* utility function to initialize ui_steps; should be moved to constructor
 * when breaking API/ABI */
void gegl_param_spec_double_set_steps (GeglParamSpecDouble *pspec,
                                       gdouble              small_step,
                                       gdouble              big_step);

void gegl_param_spec_double_set_digits (GeglParamSpecDouble *pspec,
                                        gint                 digits);

/*
 * GEGL_TYPE_PARAM_INT
 */

#define GEGL_TYPE_PARAM_INT           (gegl_param_int_get_type ())
#define GEGL_PARAM_SPEC_INT(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_INT, GeglParamSpecInt))
#define GEGL_IS_PARAM_SPEC_INT(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_INT))


struct _GeglParamSpecInt
{
  GParamSpecInt parent_instance;
  gint          ui_minimum; /* reasonable range to present to user */
  gint          ui_maximum;
  gdouble       ui_gamma;
  gint          ui_step_small;
  gint          ui_step_big;
};

GType        gegl_param_int_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_int:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @ui_minimum: minimum value a user should be allowed to input
 * @ui_maximum: maximum value a user should be allowed to input
 * @ui_gamma: the gamma that should be used when adjusting the value
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecInt instance.
 *
 * Return value: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_int (const gchar *name,
                                  const gchar *nick,
                                  const gchar *blurb,
                                  gint         minimum,
                                  gint         maximum,
                                  gint         default_value,
                                  gint         ui_minimum,
                                  gint         ui_maximum,
                                  gdouble      ui_gamma,
                                  GParamFlags  flags);


void gegl_param_spec_int_set_steps (GeglParamSpecInt  *pspec,
                                    gint               small_step,
                                    gint               big_step);

/*
 * GEGL_TYPE_PARAM_STRING
 */

#define GEGL_TYPE_PARAM_STRING           (gegl_param_string_get_type ())
#define GEGL_PARAM_SPEC_STRING(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_STRING, GeglParamSpecString))
#define GEGL_IS_PARAM_SPEC_STRING(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_STRING))

struct _GeglParamSpecString
{
  GParamSpecString parent_instance;

  guint            no_validate : 1;
  guint            null_ok     : 1;
};

GType        gegl_param_string_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_string:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @no_validate: true if the string should be validated with g_utf8_validate
 * @null_ok: true if the string can be NULL
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecString instance.
 *
 * Return value: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_string     (const gchar *name,
                                         const gchar *nick,
                                         const gchar *blurb,
                                         gboolean     no_validate,
                                         gboolean     null_ok,
                                         const gchar *default_value,
                                         GParamFlags  flags);


/*
 * GEGL_TYPE_PARAM_FILEPATH
 */

#define GEGL_TYPE_PARAM_FILE_PATH           (gegl_param_file_path_get_type ())
#define GEGL_PARAM_SPEC_FILE_PATH(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_FILE_PATH, GeglParamSpecFilePath))
#define GEGL_IS_PARAM_SPEC_FILE_PATH(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_FILE_PATH))

typedef struct _GeglParamSpecFilePath GeglParamSpecFilePath;

struct _GeglParamSpecFilePath
{
  GParamSpecString parent_instance;

  guint            no_validate : 1;
  guint            null_ok     : 1;
};

GType        gegl_param_file_path_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_file_path:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @no_validate: true if the string should be validated with g_utf8_validate
 * @null_ok: true if the string can be NULL
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecFilePath instance.
 *
 * Return value: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_file_path (const gchar *name,
                                        const gchar *nick,
                                        const gchar *blurb,
                                        gboolean     no_validate,
                                        gboolean     null_ok,
                                        const gchar *default_value,
                                        GParamFlags  flags);


/*
 * GEGL_TYPE_PARAM_MULTILINE
 */

#define GEGL_TYPE_PARAM_MULTILINE           (gegl_param_multiline_get_type ())
#define GEGL_PARAM_SPEC_MULTILINE(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_MULTILINE, GeglParamSpecMultiline))
#define GEGL_IS_PARAM_SPEC_MULTILINE(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_MULTILINE))

typedef struct _GeglParamSpecMultiline GeglParamSpecMultiline;

struct _GeglParamSpecMultiline
{
  GParamSpecString parent_instance;

  guint            no_validate : 1;
  guint            null_ok     : 1;
};

GType        gegl_param_multiline_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_multiline:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecMultiline instance.
 *
 * Return value: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_multiline (const gchar *name,
                                        const gchar *nick,
                                        const gchar *blurb,
                                        const gchar *default_value,
                                        GParamFlags  flags);



/*
 * GEGL_TYPE_PARAM_ENUM
 */

#define GEGL_TYPE_PARAM_ENUM           (gegl_param_enum_get_type ())
#define GEGL_PARAM_SPEC_ENUM(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_ENUM, GeglParamSpecEnum))

#define GEGL_IS_PARAM_SPEC_ENUM(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_ENUM))

typedef struct _GeglParamSpecEnum GeglParamSpecEnum;

struct _GeglParamSpecEnum
{
  GParamSpecEnum  parent_instance;

  GSList         *excluded_values;
};

GType        gegl_param_enum_get_type     (void) G_GNUC_CONST;

/**
 * gegl_param_spec_enum:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @enum_type: the enum type to get valid values from
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecEnum instance.
 *
 * Return value: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_enum         (const gchar       *name,
                                           const gchar       *nick,
                                           const gchar       *blurb,
                                           GType              enum_type,
                                           gint               default_value,
                                           GParamFlags        flags);

void   gegl_param_spec_enum_exclude_value (GeglParamSpecEnum *espec,
                                           gint               value);



/*
 * GEGL_TYPE_PARAM_SEED
 */

#define GEGL_TYPE_PARAM_SEED           (gegl_param_seed_get_type ())
#define GEGL_PARAM_SPEC_SEED(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_SEED, GeglParamSpecSeed))
#define GEGL_IS_PARAM_SPEC_SEED(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_SEED))

struct _GeglParamSpecSeed
{
  GParamSpecInt parent_instance;

  gint          ui_minimum;
  gint          ui_maximum;
};

GType        gegl_param_seed_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_seed:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecSeed instance specifying an integer random seed.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_seed (const gchar *name,
                                   const gchar *nick,
                                   const gchar *blurb,
                                   GParamFlags  flags);

/*
 * GEGL_TYPE_PARAM_FORMAT
 */

#define GEGL_TYPE_PARAM_FORMAT           (gegl_param_format_get_type ())
#define GEGL_PARAM_SPEC_FORMAT(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_FORMAT, GeglParamSpecFormat))
#define GEGL_IS_PARAM_SPEC_FORMAT(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_FORMAT))

struct _GeglParamSpecFormat
{
  GParamSpecPointer parent_instance;
};

GType        gegl_param_format_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_format:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecFormat instance specifying a Babl format.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_format (const gchar *name,
                                     const gchar *nick,
                                     const gchar *blurb,
                                     GParamFlags  flags);



/*
 * GEGL_TYPE_PARAM_URI
 */

#define GEGL_TYPE_PARAM_URI           (gegl_param_uri_get_type ())
#define GEGL_PARAM_SPEC_URI(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GEGL_TYPE_PARAM_URI, GeglParamSpecUri))
#define GEGL_IS_PARAM_SPEC_URI(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), GEGL_TYPE_PARAM_URI))

typedef struct _GeglParamSpecUri GeglParamSpecUri;

struct _GeglParamSpecUri
{
  GParamSpecString parent_instance;

  guint            no_validate : 1;
  guint            null_ok     : 1;
};

GType        gegl_param_uri_get_type (void) G_GNUC_CONST;

/**
 * gegl_param_spec_uri:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @no_validate: true if the string should be validated with g_utf8_validate
 * @null_ok: true if the string can be NULL
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #GeglParamSpecUri instance.
 *
 * Return value: (transfer full): a newly created parameter specification
 */
GParamSpec * gegl_param_spec_uri (const gchar *name,
                                        const gchar *nick,
                                        const gchar *blurb,
                                        gboolean     no_validate,
                                        gboolean     null_ok,
                                        const gchar *default_value,
                                        GParamFlags  flags);



G_END_DECLS
#endif  /*  __GEGL_PARAM_SPECS_H__  */
