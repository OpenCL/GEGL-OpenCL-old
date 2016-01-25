/* gegl-chant contains incantations to that produce the boilerplate
 * needed to write GEGL operation plug-ins. It abstracts away inheritance
 * by giving a limited amount of base classes, and reduced creation of
 * a properties struct and registration of properties for that structure to
 * a minimum amount of code through use of the C preprocessor. You should
 * look at the operations implemented using chanting (they #include this file)
 * to see how it is used in practice.
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
 * 2006-2008 © Øyvind Kolås.
 */

#ifndef GEGL_CHANT_C_FILE
#error "GEGL_CHANT_C_FILE not defined"
#endif

#include <gegl-plugin.h>

G_BEGIN_DECLS

GType gegl_chant_get_type (void);
typedef struct _GeglChantO  GeglChantO;
typedef struct _GeglChant   GeglChant;

static void gegl_chant_register_type       (GTypeModule *module);
static void gegl_chant_init_properties     (GeglChant   *self);
static void gegl_chant_class_intern_init   (gpointer     klass);
static gpointer gegl_chant_parent_class = NULL;

#define GEGL_DEFINE_DYNAMIC_OPERATION(T_P)  GEGL_DEFINE_DYNAMIC_OPERATION_EXTENDED (GEGL_CHANT_C_FILE, GeglChant, gegl_chant, T_P, 0, {})
#define GEGL_DEFINE_DYNAMIC_OPERATION_EXTENDED(C_FILE, TypeName, type_name, TYPE_PARENT, flags, CODE) \
static void     type_name##_init              (TypeName        *self);  \
static void     type_name##_class_init        (TypeName##Class *klass); \
static void     type_name##_class_finalize    (TypeName##Class *klass); \
static GType    type_name##_type_id = 0;                                \
static void     type_name##_class_chant_intern_init (gpointer klass)    \
  {                                                                     \
    gegl_chant_parent_class = g_type_class_peek_parent (klass);         \
    gegl_chant_class_intern_init (klass);                               \
    type_name##_class_init ((TypeName##Class*) klass);                  \
  }                                                                     \
GType                                                                   \
type_name##_get_type (void)                                             \
  {                                                                     \
    return type_name##_type_id;                                         \
  }                                                                     \
static void                                                             \
type_name##_register_type (GTypeModule *type_module)                    \
  {                                                                     \
    gchar  tempname[256];                                               \
    gchar *p;                                                           \
    const GTypeInfo g_define_type_info =                                \
    {                                                                   \
      sizeof (TypeName##Class),                                         \
      (GBaseInitFunc) NULL,                                             \
      (GBaseFinalizeFunc) NULL,                                         \
      (GClassInitFunc) type_name##_class_chant_intern_init,             \
      (GClassFinalizeFunc) type_name##_class_finalize,                  \
      NULL,   /* class_data */                                          \
      sizeof (TypeName),                                                \
      0,      /* n_preallocs */                                         \
      (GInstanceInitFunc) type_name##_init,                             \
      NULL    /* value_table */                                         \
    };                                                                  \
    g_snprintf (tempname, sizeof (tempname),                            \
                "%s", #TypeName GEGL_CHANT_C_FILE);                     \
    for (p = tempname; *p; p++)                                         \
      if (*p=='.') *p='_';                                              \
                                                                        \
    type_name##_type_id = g_type_module_register_type (type_module,     \
                                                       TYPE_PARENT,     \
                                                       tempname,        \
                                                       &g_define_type_info, \
                                                       (GTypeFlags) flags); \
    { CODE ; }                                                          \
  }


#define GEGL_CHANT_PROPERTIES(op) \
    ((GeglChantO *) (((GeglChant *) (op))->properties))

/****************************************************************************/


#ifdef GEGL_CHANT_TYPE_OPERATION
struct _GeglChant
{
  GeglOperation parent_instance;
  gpointer      properties;
};

typedef struct
{
  GeglOperationClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION)
#endif

#ifdef GEGL_CHANT_TYPE_META
#include <operation/gegl-operation-meta.h>
struct _GeglChant
{
  GeglOperationMeta parent_instance;
  gpointer          properties;
};

typedef struct
{
  GeglOperationMetaClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_META)
#endif

#ifdef GEGL_CHANT_TYPE_SOURCE
struct _GeglChant
{
  GeglOperationSource parent_instance;
  gpointer            properties;
};

typedef struct
{
  GeglOperationSourceClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_SOURCE)
#endif

#ifdef GEGL_CHANT_TYPE_SINK
struct _GeglChant
{
  GeglOperationSink parent_instance;
  gpointer          properties;
};

typedef struct
{
  GeglOperationSinkClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_SINK)
#endif

#ifdef GEGL_CHANT_TYPE_FILTER
struct _GeglChant
{
  GeglOperationFilter parent_instance;
  gpointer            properties;
};

typedef struct
{
  GeglOperationFilterClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_FILTER)
#endif

#ifdef GEGL_CHANT_TYPE_COMPOSER
struct _GeglChant
{
  GeglOperationComposer parent_instance;
  gpointer              properties;
};

typedef struct
{
  GeglOperationComposerClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_COMPOSER)

#endif

#ifdef GEGL_CHANT_TYPE_COMPOSER3
struct _GeglChant
{
  GeglOperationComposer3 parent_instance;
  gpointer               properties;
};

typedef struct
{
  GeglOperationComposer3Class parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_COMPOSER3)

#endif

#ifdef GEGL_CHANT_TYPE_POINT_FILTER
struct _GeglChant
{
  GeglOperationPointFilter parent_instance;
  gpointer                 properties;
};

typedef struct
{
  GeglOperationPointFilterClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_POINT_FILTER)

#endif


#ifdef GEGL_CHANT_TYPE_POINT_RENDER
struct _GeglChant
{
  GeglOperationPointRender parent_instance;
  gpointer                 properties;
};

typedef struct
{
  GeglOperationPointRenderClass parent_class;
} GeglChantClass;

GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_POINT_RENDER)

#endif


#ifdef GEGL_CHANT_TYPE_TEMPORAL
struct _GeglChant
{
  GeglOperationTemporal parent_instance;
  gpointer              properties;
};

typedef struct
{
  GeglOperationTemporalClass parent_class;
} GeglChantClass;
GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_TEMPORAL)
#endif

#ifdef GEGL_CHANT_TYPE_AREA_FILTER
struct _GeglChant
{
  GeglOperationAreaFilter parent_instance;
  gpointer                properties;
};

typedef struct
{
  GeglOperationAreaFilterClass parent_class;
} GeglChantClass;
GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_AREA_FILTER)
#endif


#ifdef GEGL_CHANT_TYPE_POINT_COMPOSER
struct _GeglChant
{
  GeglOperationPointComposer parent_instance;
  gpointer                   properties;
};

typedef struct
{
  GeglOperationPointComposerClass parent_class;
} GeglChantClass;
GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_POINT_COMPOSER)
#endif

#ifdef GEGL_CHANT_TYPE_POINT_COMPOSER3
struct _GeglChant
{
  GeglOperationPointComposer3 parent_instance;
  gpointer                    properties;
};

typedef struct
{
  GeglOperationPointComposer3Class parent_class;
} GeglChantClass;
GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_POINT_COMPOSER3)
#endif

#define GEGL_CHANT(obj)  ((GeglChant*)(obj))

/* if GEGL_CHANT_CUSTOM is defined you have to provide the following
 * code or your own implementation of it
 */
#ifndef GEGL_CHANT_CUSTOM
static void
gegl_chant_init (GeglChant *self)
{
  gegl_chant_init_properties (self);
}

static void
gegl_chant_class_finalize (GeglChantClass *self)
{
}

static const GeglModuleInfo modinfo =
{
  GEGL_MODULE_ABI_VERSION
};

/* prototypes added to silence warnings from gcc for -Wmissing-prototypes*/
gboolean                gegl_module_register (GTypeModule *module);
const GeglModuleInfo  * gegl_module_query    (GTypeModule *module);

G_MODULE_EXPORT const GeglModuleInfo *
gegl_module_query (GTypeModule *module)
{
  return &modinfo;
}

G_MODULE_EXPORT gboolean
gegl_module_register (GTypeModule *module)
{
  gegl_chant_register_type (module);

  return TRUE;
}
#endif

/* enum registration */
#define gegl_chant_int(name, nick, min, max, def, blurb)
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)
#define gegl_chant_double(name, nick, min, max, def, blurb)
#define gegl_chant_double_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)
#define gegl_chant_boolean(name, nick, def, blurb)
#define gegl_chant_string(name, nick, def, blurb)
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)
#define gegl_chant_file_path(name, nick, def, blurb)
#define gegl_chant_multiline(name, nick, def, blurb)
#define gegl_chant_object(name,nick,  blurb)
#define gegl_chant_pointer(name, nick, blurb)
#define gegl_chant_format(name, nick, blurb)
#define gegl_chant_color(name, nick, def, blurb)
#define gegl_chant_curve(name, nick, blurb)
#define gegl_chant_seed(name, rand_name, nick, blurb)
#define gegl_chant_path(name, nick, blurb)
#define gegl_chant_register_enum(enum_name) \
typedef enum {

#define enum_value(value, nick) \
  value ,

#define gegl_chant_register_enum_end(enum) \
} enum ;

#include GEGL_CHANT_C_FILE


#undef gegl_chant_register_enum
#undef enum_value
#undef gegl_chant_register_enum_end

#define gegl_chant_register_enum(enum_name)       \
GType enum_name ## _get_type (void) G_GNUC_CONST; \
GType enum_name ## _get_type (void)               \
{                                                 \
  static GType etype = 0;                         \
  if (etype == 0) {                               \
    static const GEnumValue values[] = {

#define enum_value(value, nick) \
      { value, nick, nick },

#define gegl_chant_register_enum_end(enum)                \
      { 0, NULL, NULL }                                   \
    };                                                    \
    etype = g_enum_register_static (#enum, values);       \
  }                                                       \
  return etype;                                           \
}                                                         \

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path
#undef gegl_chant_register_enum
#undef enum_value
#undef gegl_chant_register_enum_end
#define gegl_chant_register_enum(enum_name)
#define enum_value(value, nick)
#define gegl_chant_register_enum_end(enum)

/* Properties */

struct _GeglChantO
{
  gpointer chant_data; /* Unused by the chanting framework can be used by operations
                        * for storage of a private struct, (remember to clean up
                        * in finalize). Also serves as a filler making sure that we
                        * do not create an empty struct if there are no chanted properties.
                        */
#define gegl_chant_int(name, nick, min, max, def, blurb)                        gint               name;
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)     gint               name;
#define gegl_chant_double(name, nick, min, max, def, blurb)                     gdouble            name;
#define gegl_chant_double_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb) gdouble   name;
#define gegl_chant_boolean(name, nick, def, blurb)                              gboolean           name;
#define gegl_chant_string(name, nick, def, blurb)                               gchar             *name;
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)                    enum               name;
#define gegl_chant_file_path(name, nick, def, blurb)                            gchar             *name;
#define gegl_chant_multiline(name, nick, def, blurb)                            gchar             *name;
#define gegl_chant_object(name,nick,  blurb)                                    GObject           *name;
#define gegl_chant_pointer(name, nick, blurb)                                   gpointer           name;
#define gegl_chant_format(name, nick, blurb)                                    gpointer           name;
#define gegl_chant_color(name, nick, def, blurb)                                GeglColor         *name;
#define gegl_chant_curve(name, nick, blurb)                                     GeglCurve         *name;
#define gegl_chant_seed(name, rand_name, nick, blurb)                           gint               name;\
                                                                                GeglRandom        *rand_name;
#define gegl_chant_path(name, nick, blurb)                                      GeglPath          *name;\
                                                                                gulong path_changed_handler;

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path
};

#define GEGL_CHANT_OPERATION(obj) ((Operation*)(obj))

enum
{
  PROP_0,
#define gegl_chant_int(name, nick, min, max, def, blurb)                        PROP_##name,
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)     PROP_##name,
#define gegl_chant_double(name, nick, min, max, def, blurb)                     PROP_##name,
#define gegl_chant_double_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb) PROP_##name,
#define gegl_chant_boolean(name, nick, def, blurb)                              PROP_##name,
#define gegl_chant_string(name, nick, def, blurb)                               PROP_##name,
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)                    PROP_##name,
#define gegl_chant_file_path(name, nick, def, blurb)                            PROP_##name,
#define gegl_chant_multiline(name, nick, def, blurb)                            PROP_##name,
#define gegl_chant_object(name, nick, blurb)                                    PROP_##name,
#define gegl_chant_pointer(name, nick, blurb)                                   PROP_##name,
#define gegl_chant_format(name, nick, blurb)                                    PROP_##name,
#define gegl_chant_color(name, nick, def, blurb)                                PROP_##name,
#define gegl_chant_curve(name, nick, blurb)                                     PROP_##name,
#define gegl_chant_seed(name, rand_name, nick, blurb)                           PROP_##name,
#define gegl_chant_path(name, nick, blurb)                                      PROP_##name,

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path
  PROP_LAST
};

static void
get_property (GObject      *gobject,
              guint         property_id,
              GValue       *value,
              GParamSpec   *pspec)
{
  GeglChantO *properties;

  properties = GEGL_CHANT_PROPERTIES(gobject);

  switch (property_id)
  {
#define gegl_chant_int(name, nick, min, max, def, blurb)      \
    case PROP_##name:                                         \
      g_value_set_int (value, properties->name);              \
      break;
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb) \
    case PROP_##name:                                         \
      g_value_set_int (value, properties->name);              \
      break;
#define gegl_chant_double(name,nick,  min, max, def, blurb)   \
    case PROP_##name:                                         \
      g_value_set_double (value, properties->name);           \
      break;
#define gegl_chant_double_ui(name,nick,  min, max, def, ui_min, ui_max, ui_gamma, blurb)   \
    case PROP_##name:                                         \
      g_value_set_double (value, properties->name);           \
      break;
#define gegl_chant_boolean(name, nick, def, blurb)            \
    case PROP_##name:                                         \
      g_value_set_boolean (value, properties->name);          \
      break;
#define gegl_chant_string(name, nick, def, blurb)             \
    case PROP_##name:                                         \
      g_value_set_string (value, properties->name);           \
      break;
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)  \
    case PROP_##name:                                         \
      g_value_set_enum (value, properties->name);             \
      break;
#define gegl_chant_file_path(name, nick, def, blurb)          \
    case PROP_##name:                                         \
      g_value_set_string (value, properties->name);           \
      break;
#define gegl_chant_multiline(name, nick, def, blurb)          \
    case PROP_##name:                                         \
      g_value_set_string (value, properties->name);           \
      break;
#define gegl_chant_object(name, nick, blurb)                  \
    case PROP_##name:                                         \
      g_value_set_object (value, properties->name);           \
      break;
#define gegl_chant_pointer(name, nick, blurb)                 \
    case PROP_##name:                                         \
      g_value_set_pointer (value, properties->name);          \
      break;
#define gegl_chant_format(name, nick, blurb)                 \
    case PROP_##name:                                         \
      g_value_set_pointer (value, properties->name);          \
      break;
#define gegl_chant_color(name, nick, def, blurb)              \
    case PROP_##name:                                         \
      g_value_set_object (value, properties->name);           \
      break;
#define gegl_chant_curve(name, nick, blurb)                   \
    case PROP_##name:                                         \
      g_value_set_object (value, properties->name);           \
      break;
#define gegl_chant_seed(name, rand_name, nick, blurb)         \
    case PROP_##name:                                         \
      g_value_set_int (value, properties->name);              \
      break;
#define gegl_chant_path(name, nick, blurb)                    \
    case PROP_##name:                                         \
      g_value_set_object (value, properties->name);           \
      break;

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, property_id, pspec);
      break;
  }
  properties ++; /* evil hack to silence gcc */
}

static void
set_property (GObject      *gobject,
              guint         property_id,
              const GValue *value,
              GParamSpec   *pspec)
{
  GeglChantO *properties;

  properties = GEGL_CHANT_PROPERTIES(gobject);

  switch (property_id)
  {
#define gegl_chant_int(name, nick, min, max, def, blurb)              \
    case PROP_##name:                                                 \
      properties->name = g_value_get_int (value);                     \
      break;
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb) \
    case PROP_##name:                                                 \
      properties->name = g_value_get_int (value);                     \
      break;
#define gegl_chant_double(name, nick, min, max, def, blurb)           \
    case PROP_##name:                                                 \
      properties->name = g_value_get_double (value);                  \
      break;
#define gegl_chant_double_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb) \
    case PROP_##name:                                                 \
      properties->name = g_value_get_double (value);                  \
      break;
#define gegl_chant_boolean(name, nick, def, blurb)                    \
    case PROP_##name:                                                 \
      properties->name = g_value_get_boolean (value);                 \
      break;
#define gegl_chant_string(name, nick, def, blurb)                     \
    case PROP_##name:                                                 \
      if (properties->name)                                           \
        g_free (properties->name);                                    \
      properties->name = g_value_dup_string (value);                  \
      break;
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)      \
    case PROP_##name:                                                 \
      properties->name = g_value_get_enum (value);                    \
      break;
#define gegl_chant_file_path(name, nick, def, blurb)                  \
    case PROP_##name:                                                 \
      if (properties->name)                                           \
        g_free (properties->name);                                    \
      properties->name = g_value_dup_string (value);                  \
      break;
#define gegl_chant_multiline(name, nick, def, blurb)                  \
    case PROP_##name:                                                 \
      if (properties->name)                                           \
        g_free (properties->name);                                    \
      properties->name = g_value_dup_string (value);                  \
      break;
#define gegl_chant_object(name, nick, blurb)                          \
    case PROP_##name:                                                 \
      if (properties->name != NULL)                                   \
         g_object_unref (properties->name);                           \
      properties->name = g_value_dup_object (value);                  \
      break;
#define gegl_chant_pointer(name, nick, blurb)                         \
    case PROP_##name:                                                 \
      properties->name = g_value_get_pointer (value);                 \
      break;
#define gegl_chant_format(name, nick, blurb)                          \
    case PROP_##name:                                                 \
      properties->name = g_value_get_pointer (value);                 \
      break;
#define gegl_chant_color(name, nick, def, blurb)                      \
    case PROP_##name:                                                 \
      if (properties->name != NULL)                                   \
         g_object_unref (properties->name);                           \
      properties->name = g_value_dup_object (value);                  \
      break;
#define gegl_chant_curve(name, nick, blurb)                           \
    case PROP_##name:                                                 \
      if (properties->name != NULL)                                   \
         g_object_unref (properties->name);                           \
      properties->name = g_value_dup_object (value);                  \
      break;
#define gegl_chant_seed(name, rand_name, nick, blurb)                 \
    case PROP_##name:                                                 \
      properties->name = g_value_get_int (value);                     \
      if (!properties->rand_name)                                     \
        properties->rand_name = gegl_random_new_with_seed (properties->name);\
      else                                                            \
        gegl_random_set_seed (properties->rand_name, properties->name);\
      break;
#define gegl_chant_path(name, nick, blurb)                            \
    case PROP_##name:                                                 \
      if (properties->name != NULL)                                   \
        {                                                             \
          if (properties->path_changed_handler)                       \
            g_signal_handler_disconnect (G_OBJECT (properties->name), \
                                         properties->path_changed_handler); \
          properties->path_changed_handler = 0;                       \
          g_object_unref (properties->name);                          \
        }                                                             \
      properties->name = g_value_dup_object (value);                  \
      if (properties->name != NULL)                                   \
        {                                                             \
          properties->path_changed_handler =                          \
            g_signal_connect (G_OBJECT (properties->name), "changed", \
                              G_CALLBACK(path_changed), gobject);     \
         }                                                            \
      break; 

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, property_id, pspec);
      break;
  }

  properties ++; /* evil hack to silence gcc */
}

static void gegl_chant_destroy_notify (gpointer data)
{
  GeglChantO *properties = GEGL_CHANT_PROPERTIES (data);

#define gegl_chant_int(name, nick, min, max, def, blurb)
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)
#define gegl_chant_double(name, nick, min, max, def, blurb)
#define gegl_chant_double_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)
#define gegl_chant_boolean(name, nick, def, blurb)
#define gegl_chant_string(name, nick, def, blurb)   \
  if (properties->name)                             \
    {                                               \
      g_free (properties->name);                    \
      properties->name = NULL;                      \
    }
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)
#define gegl_chant_file_path(name, nick, def, blurb) \
  if (properties->name)                             \
    {                                               \
      g_free (properties->name);                    \
      properties->name = NULL;                      \
    }
#define gegl_chant_multiline(name, nick, def, blurb)\
  if (properties->name)                             \
    {                                               \
      g_free (properties->name);                    \
      properties->name = NULL;                      \
    }
#define gegl_chant_object(name, nick, blurb)        \
  if (properties->name)                             \
    {                                               \
      g_object_unref (properties->name);            \
      properties->name = NULL;                      \
    }
#define gegl_chant_pointer(name, nick, blurb)
#define gegl_chant_format(name, nick, blurb)
#define gegl_chant_color(name, nick, def, blurb)    \
  if (properties->name)                             \
    {                                               \
      g_object_unref (properties->name);            \
      properties->name = NULL;                      \
    }
#define gegl_chant_curve(name, nick, blurb)         \
  if (properties->name)                             \
    {\
      g_object_unref (properties->name);            \
      properties->name = NULL;                      \
    }
#define gegl_chant_seed(name, rand_name, nick, blurb)\
  if (properties->rand_name)                         \
    {                                                \
      gegl_random_free (properties->rand_name);      \
      properties->rand_name = NULL;                  \
    }
#define gegl_chant_path(name, nick, blurb)          \
  if (properties->name)                             \
    {                                               \
      g_object_unref (properties->name);            \
      properties->name = NULL;                      \
    }

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path

  g_slice_free (GeglChantO, properties);
}

static GObject *
gegl_chant_constructor (GType                  type,
                        guint                  n_construct_properties,
                        GObjectConstructParam *construct_properties)
{
  GObject *obj;
  GeglChantO *properties;

  obj = G_OBJECT_CLASS (gegl_chant_parent_class)->constructor (
            type, n_construct_properties, construct_properties);

  /* create dummy colors and vectors */
  properties = GEGL_CHANT_PROPERTIES (obj);

#define gegl_chant_int(name, nick, min, max, def, blurb)
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)
#define gegl_chant_double(name, nick, min, max, def, blurb)
#define gegl_chant_double_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)
#define gegl_chant_boolean(name, nick, def, blurb)
#define gegl_chant_string(name, nick, def, blurb)
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)
#define gegl_chant_file_path(name, nick, def, blurb)
#define gegl_chant_multiline(name, nick, def, blurb)
#define gegl_chant_object(name, nick, blurb)
#define gegl_chant_pointer(name, nick, blurb)
#define gegl_chant_format(name, nick, blurb)
#define gegl_chant_color(name, nick, def, blurb)              \
    if (properties->name == NULL) \
    {properties->name = gegl_color_new(def?def:"black");}
#define gegl_chant_seed(name, rand_name, nick, blurb)         \
    if (properties->rand_name == NULL)                        \
      {properties->rand_name = gegl_random_new_with_seed (0);}
#define gegl_chant_path(name, nick, blurb)
#define gegl_chant_curve(name, nick, blurb)

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path

  g_object_set_data_full (obj, "chant-data", obj, gegl_chant_destroy_notify);
  properties ++; /* evil hack to silence gcc */
  return obj;
}

static void
gegl_chant_class_intern_init (gpointer klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = set_property;
  object_class->get_property = get_property;
  object_class->constructor  = gegl_chant_constructor;

#define gegl_chant_int(name, nick, min, max, def, blurb)                     \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   g_param_spec_int (#name, nick, blurb,     \
                                                     min, max, def,          \
                                                     (GParamFlags) (         \
                                                     G_PARAM_READWRITE |     \
                                                     G_PARAM_CONSTRUCT |     \
                                                     GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_int_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)  \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_int (#name, nick, blurb,     \
                                                     min, max, def, ui_min, ui_max, ui_gamma, \
                                                     (GParamFlags) (         \
                                                     G_PARAM_READWRITE |     \
                                                     G_PARAM_CONSTRUCT |     \
                                                     GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_double(name, nick, min, max, def, blurb)                  \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   g_param_spec_double (#name, nick, blurb,  \
                                                        min, max, def,       \
                                                        (GParamFlags) (      \
                                                        G_PARAM_READWRITE |  \
                                                        G_PARAM_CONSTRUCT |  \
                                                        GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_double_ui(name, nick, min, max, def, ui_min, ui_max, ui_gamma, blurb)                  \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_double (#name, nick, blurb,  \
                                                           min, max, def, ui_min, ui_max, ui_gamma, \
                                                           (GParamFlags) (      \
                                                           G_PARAM_READWRITE |  \
                                                           G_PARAM_CONSTRUCT |  \
                                                           GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_boolean(name, nick, def, blurb)                           \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   g_param_spec_boolean (#name, nick, blurb, \
                                                         def,                \
                                                         (GParamFlags) (     \
                                                         G_PARAM_READWRITE | \
                                                         G_PARAM_CONSTRUCT | \
                                                         GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_string(name, nick, def, blurb)                            \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   g_param_spec_string (#name, nick, blurb,  \
                                                        def,                 \
                                                        (GParamFlags) (      \
                                                        G_PARAM_READWRITE |  \
                                                        G_PARAM_CONSTRUCT |  \
                                                        GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_enum(name, nick, enum, enum_name, def, blurb)                 \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   g_param_spec_enum (#name, nick, blurb,    \
                                                      enum_name ## _get_type (), \
                                                      def,                   \
                                                      (GParamFlags) (        \
                                                      G_PARAM_READWRITE |    \
                                                      G_PARAM_CONSTRUCT |    \
                                                      GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_file_path(name, nick, def, blurb)                              \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_file_path (#name, nick, blurb, \
                                                         FALSE, FALSE,       \
                                                         def,                \
                                                         (GParamFlags) (     \
                                                         G_PARAM_READWRITE | \
                                                         G_PARAM_CONSTRUCT | \
                                                         GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_multiline(name, nick, def, blurb)                         \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_multiline (#name, nick, blurb, \
                                                         def,                \
                                                         (GParamFlags) (     \
                                                         G_PARAM_READWRITE | \
                                                         G_PARAM_CONSTRUCT | \
                                                         GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_object(name, nick, blurb)                                 \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   g_param_spec_object (#name, nick, blurb,  \
                                                        G_TYPE_OBJECT,       \
                                                        (GParamFlags) (      \
                                                        G_PARAM_READWRITE |  \
                                                        G_PARAM_CONSTRUCT |  \
                                                        GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_pointer(name, nick, blurb)                                \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   g_param_spec_pointer (#name, nick, blurb, \
                                                        (GParamFlags) (      \
                                                        G_PARAM_READWRITE |  \
                                                        G_PARAM_CONSTRUCT |  \
                                                        GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_format(name, nick, blurb)                                 \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_format (#name, nick, blurb, \
                                                          (GParamFlags) (      \
                                                          G_PARAM_READWRITE |  \
                                                          G_PARAM_CONSTRUCT |  \
                                                          GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_color(name, nick, def, blurb)                             \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_color_from_string (#name, nick, blurb,\
                                                          def,               \
                                                          (GParamFlags) (    \
                                                          G_PARAM_READWRITE |\
                                                          G_PARAM_CONSTRUCT | \
                                                          GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_seed(name, rand_name, nick, blurb)                        \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_seed (#name, nick, blurb, \
                                                     (GParamFlags) (         \
                                                     G_PARAM_READWRITE |     \
                                                     G_PARAM_CONSTRUCT |     \
                                                     GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_path(name, nick, blurb)                                 \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_path (#name, nick, blurb,\
                                                           NULL,             \
                                                          (GParamFlags) (    \
                                                          G_PARAM_READWRITE |\
                                                          G_PARAM_CONSTRUCT |\
                                                          GEGL_PARAM_PAD_INPUT)));
#define gegl_chant_curve(name, nick, blurb)                                  \
  { GeglCurve *_gegl_chant_default_curve = gegl_curve_new_default ();        \
  g_object_class_install_property (object_class, PROP_##name,                \
                                   gegl_param_spec_curve (#name, nick, blurb,\
                                                          _gegl_chant_default_curve,\
                                                          (GParamFlags) (    \
                                                          G_PARAM_READWRITE |\
                                                          G_PARAM_CONSTRUCT |\
                                                          GEGL_PARAM_PAD_INPUT)));\
  g_object_unref (_gegl_chant_default_curve); }

#include GEGL_CHANT_C_FILE

#undef gegl_chant_int
#undef gegl_chant_int_ui
#undef gegl_chant_double
#undef gegl_chant_double_ui
#undef gegl_chant_boolean
#undef gegl_chant_string
#undef gegl_chant_enum
#undef gegl_chant_file_path
#undef gegl_chant_multiline
#undef gegl_chant_object
#undef gegl_chant_pointer
#undef gegl_chant_format
#undef gegl_chant_color
#undef gegl_chant_curve
#undef gegl_chant_seed
#undef gegl_chant_path
}


static void
gegl_chant_init_properties (GeglChant *self)
{
  self->properties = g_slice_new0 (GeglChantO);
}

G_END_DECLS
/****************************************************************************/
