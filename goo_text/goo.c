
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
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 Beaver goo text with GEGL
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_color (color, _("Color"), "#00f73b")
    description (_("The color to paint over the input"))
property_double (tile_size, _("Goo size"), 6.0)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (3.40, 15.0)
    ui_meta     ("unit", "pixel-distance")

property_double (tile_saturation, _("Goo internal saturation"), 2.0)
    description (_("Expand tiles by this amount"))
    value_range (2, 2.50)


property_seed (seed, _("Random seed"), rand)

enum_start (gegl_wind1_direction)
  enum_value (GEGL_WIND_DIRECTION_LEFT1, "left", N_("Left"))
  enum_value (GEGL_WIND_DIRECTION_RIGHT1, "right", N_("Right"))
  enum_value (GEGL_WIND_DIRECTION_TOP1, "top", N_("Top"))
  enum_value (GEGL_WIND_DIRECTION_BOTTOM1, "bottom", N_("Bottom"))
enum_end (GeglWindDirection1)

property_enum (direction, _("Direction"),
               GeglWindDirection1, gegl_wind1_direction,
               GEGL_WIND_DIRECTION_BOTTOM1)
  description (_("Direction of the effect"))


property_int (strength, _("Length of Goo trail"), 35)
 description (_("Higher values increase the magnitude of the effect"))
 value_range (1, 500)

property_int  (median, _("Radius"), 3)
  value_range (3, 6)
  ui_range    (3, 6)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_double (alpha, _("Threshold"), 0.09)
    value_range (00.1, 0.09)
    ui_range    (-1, 2)
    description(_("Scalar threshold level (overridden if an auxiliary input buffer is provided.)."))


property_int (box, _("Smooth"), 2)
   description(_("Radius of square pixel region, (width and height will be radius*2+1)"))
   value_range (1, 2)
   ui_range    (1, 2)
   ui_gamma   (1.5)

property_color (color2, _("Color 2"), "#00f73b")
    description (_("The color to paint over the input"))
property_double (tile_size2, _("Goo size 2"), 6.9)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (4.8, 8.0)
    ui_meta     ("unit", "pixel-distance")

property_double (tile_saturation2, _("Goo internal saturation"), 1.1)
    description (_("Expand tiles by this amount"))
    value_range (1.0, 1.2)


property_seed (seed2, _("Random seed"), rand2)

property_int  (median2, _("Radius"), 3)
  value_range (3, 6)
  ui_range    (3, 6)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_int (box2, _("Smooth"), 2)
   description(_("Radius of square pixel region, (width and height will be radius*2+1)"))
   value_range (1, 2)
   ui_range    (1, 2)
   ui_gamma   (1.5)

property_double (bevel, _("Bevel external Goo"), 15.0)
  value_range (0.0, 100.0)
  ui_range (0.0, 100.0)
  ui_gamma (1.5)

property_double (bevel2, _("Bevel Goo on top"), 15.0)
  value_range (0.0, 100.0)
  ui_range (0.0, 100.0)
  ui_gamma (1.5)



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     goo
#define GEGL_OP_C_SOURCE goo.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *color, *color2, *atop, *over, *cubism, *median, *median2, *cubism2, *wind2, *alpha2, *box2, *light, *alpha, *box, *wind, *bevel, *bevel2;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  color2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  over    = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);

  atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-atop",
                                  NULL);


  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

  median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

  cubism    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);

  cubism2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);


  wind    = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzwind",
                                  NULL);
  wind2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzwind",
                                  NULL);

  alpha    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

  alpha2    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

  box    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);

  box2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);
  light = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  bevel = gegl_node_new_child (gegl,
                                  "operation", "gegl:mbd",
                                  NULL);

  bevel2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:mbd",
                                  NULL);



 gegl_node_link_many (input, atop, over, output, NULL);
 gegl_node_link_many (input, color, cubism, wind, median, alpha, bevel, box, NULL);
 gegl_node_link_many (atop, color2, light, cubism2, wind2, median2, alpha2, bevel2, box2, NULL);
gegl_node_connect_from (atop, "aux", box, "output"); 
gegl_node_connect_from (over, "aux", box2, "output"); 


  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "color2", color2, "value");
  gegl_operation_meta_redirect (operation, "seed", cubism, "seed");
  gegl_operation_meta_redirect (operation, "seed2", cubism2, "seed");
  gegl_operation_meta_redirect (operation, "tile_size2", cubism2, "tile-size");
  gegl_operation_meta_redirect (operation, "tile_size", cubism, "tile-size");
  gegl_operation_meta_redirect (operation, "tile_saturation", cubism, "tile-saturation");
  gegl_operation_meta_redirect (operation, "tile_saturation2", cubism2, "tile-saturation");
  gegl_operation_meta_redirect (operation, "direction", wind, "direction");
  gegl_operation_meta_redirect (operation, "strength", wind, "strength");
  gegl_operation_meta_redirect (operation, "median", median, "radius");
  gegl_operation_meta_redirect (operation, "median2", median2, "radius");
  gegl_operation_meta_redirect (operation, "alpha", alpha, "value");
  gegl_operation_meta_redirect (operation, "box", box, "radius");
  gegl_operation_meta_redirect (operation, "box2", box2, "radius");
  gegl_operation_meta_redirect (operation, "bevel", bevel, "radius2");
  gegl_operation_meta_redirect (operation, "bevel2", bevel2, "radius2");


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:goo-text",
    "title",       _("Goo on Text"),
    "categories",  "Artistic",
    "reference-hash", "10gz1ak3aa10aavx65421xc254001b2ac",
    "description", _("Put goo on text using GEGL. It is a strong recommendation that both colors are similar or the same "
                     ""),
    NULL);
}

#endif
