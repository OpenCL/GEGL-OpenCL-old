#include "test-common.h"

void bcontrast_minichunk(GeglBuffer *buffer);

gint
main (gint    argc,
      gchar **argv)
{
  GeglBuffer *buffer;

  gegl_init (&argc, &argv);

  g_object_set (gegl_config (), "chunk-size", 32 * 32, NULL);

  buffer = test_buffer (2048, 1024, babl_format ("RGBA float"));
  bench("bcontrast-minichunk", buffer, &bcontrast_minichunk);

  return 0;
}

void bcontrast_minichunk(GeglBuffer *buffer)
{
  GeglBuffer *buffer2;
  GeglNode   *gegl, *source, *node, *sink;

  gegl = gegl_node_new ();
  source = gegl_node_new_child (gegl, "operation", "gegl:buffer-source", "buffer", buffer, NULL);
  node = gegl_node_new_child (gegl, "operation", "gegl:brightness-contrast", "contrast", 0.2, NULL);
  sink = gegl_node_new_child (gegl, "operation", "gegl:buffer-sink", "buffer", &buffer2, NULL);

  gegl_node_link_many (source, node, sink, NULL);
  gegl_node_process (sink);
  g_object_unref (gegl);
  g_object_unref (buffer2);
}
