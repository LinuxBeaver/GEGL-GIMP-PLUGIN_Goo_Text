
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



/*

GEGL Graph of Goo Text to test without installing.


id=1
svg:dst-atop aux=[ ref=1 gegl:color-overlay value=#0b9500 cubism seed=13 tile-size=6 wind direction=bottom strength=80 median-blur radius=3 gimp:threshold-alpha value=0.09 box-blur radius=2  id=4 multiply aux=[ ref=4 lb:bevel ] ]


id=2

over aux=[ ref=2    gegl:color-overlay value=#0b9500 hue-chroma lightness=12 cubism seed=13 tile-size=11 tile-saturation=1.2 median-blur radius=3  box-blur radius=1 id=4 multiply aux=[ ref=4 lb:bevel ]  ]

 */





#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_color (color, _("Color of external goo"), "#00f73b")
    description (_("The color to paint over the input"))

property_double (tile_size, _("Increase to coat text with Goo"), 6.0)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (3.40, 15.0)
    ui_meta     ("unit", "pixel-distance")

property_double (tile_saturation, _("External goo spread"), 2.0)
    description (_("Expand tiles by this amount"))
    value_range (2, 2.50)


property_seed (seed, _("Random seed for external goo"), rand)


property_int (strength, _("Length of Goo drip"), 35)
 description (_("Higher values increase the magnitude of the effect"))
 value_range (1, 500)

property_int  (median, _("Radius of drip"), 3)
  value_range (3, 6)
  ui_range    (3, 6)
  ui_meta     ("unit", "pixel-distance")
  description (_("Median Blur Radius - Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_double (bevel, _("Bevel external Goo"), 15.0)
  value_range (0.0, 100.0)
  ui_range (0.0, 100.0)
  ui_gamma (1.5)


property_double (alpha, _("Threshold Alpha"), 0.09)
    value_range (0.09, 0.1)
    ui_range    (0.09, 0.1)
    description(_("Threshold Alpha's Scalar threshold level (overridden if an auxiliary input buffer is provided.)."))
    ui_meta     ("role", "output-extent")



property_color (color2, _("Goo on top color"), "#00f73b")
    description (_("The color to paint on the top goo"))

property_double (tile_size2, _("Goo on top size"), 5.2)
    description (_("Internal Cubism Tile Size for the goo on top"))
    value_range (4.8, 8.0)
    ui_meta     ("unit", "pixel-distance")

property_double (tile_saturation2, _("Goo on top spread"), 1.1)
    description (_("Expand cubism tiles by this amount"))
    value_range (1.0, 1.2)




property_seed (seed2, _("Random seed for goo on top"), rand2)

property_int  (median2, _("Radius for goo on top"), 3)
  value_range (3, 6)
  ui_range    (3, 6)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_int (box2, _("Smooth goo on top"), 2)
   description(_("Radius of square pixel region, (width and height will be radius*2+1)"))
   value_range (1, 2)
   ui_range    (1, 2)
   ui_gamma   (1.5)


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
  GeglNode *input, *output, *color, *color2, *atop, *over, *cubism, *median, *median2, *multiply, *multiply2, *cubism2, *wind2, *alpha2, *box2, *light, *alpha, *box, *wind, *bevel, *bevel2;


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
                                  "operation", "gegl:median-blur", "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

  median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

  cubism    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);

  cubism2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);


  wind    = gegl_node_new_child (gegl,
                                  "operation", "lb:zzwind", "direction", 3,
                                  NULL);

/* This is a hidden operation of mine that allows gegl:wind to go up to 500. Where in default it is locked at 100. This is
one of the few hidden operations that still remain. The direction number is calling the wind to go down */

  wind2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:wind",
                                  NULL);

  alpha    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

  alpha2    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

  box    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur", "radius", 2,
                                  NULL);

  box2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);
  light = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  bevel = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "bevel1", 90.0,
                                  NULL);

  bevel2 = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "bevel1", 90.0,
                                  NULL);

  multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  multiply2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);




 gegl_node_link_many (input, atop, over, output, NULL);
 gegl_node_link_many (input, color, cubism, wind, median, alpha, multiply, box, NULL);
gegl_node_connect (multiply, "aux", bevel, "output");
gegl_node_connect (multiply2, "aux", bevel2, "output");
 gegl_node_link_many (alpha, bevel, NULL);
 gegl_node_link_many (alpha2, bevel2, NULL);
 gegl_node_link_many (atop, color2, light, cubism2, wind2, median2, alpha2, multiply2, box2, NULL);
gegl_node_connect (atop, "aux", box, "output");
gegl_node_connect (over, "aux", box2, "output");


  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "color2", color2, "value");
  gegl_operation_meta_redirect (operation, "seed", cubism, "seed");
  gegl_operation_meta_redirect (operation, "seed2", cubism2, "seed");
  gegl_operation_meta_redirect (operation, "tile_size2", cubism2, "tile-size");
  gegl_operation_meta_redirect (operation, "tile_size", cubism, "tile-size");
  gegl_operation_meta_redirect (operation, "tile_saturation", cubism, "tile-saturation");
  gegl_operation_meta_redirect (operation, "tile_saturation2", cubism2, "tile-saturation");
  gegl_operation_meta_redirect (operation, "strength", wind, "strength");
  gegl_operation_meta_redirect (operation, "median", median, "radius");
  gegl_operation_meta_redirect (operation, "median2", median2, "radius");
  gegl_operation_meta_redirect (operation, "alpha", alpha, "value");
  gegl_operation_meta_redirect (operation, "box2", box2, "radius");
  gegl_operation_meta_redirect (operation, "bevel", bevel, "bevel2");
  gegl_operation_meta_redirect (operation, "bevel2", bevel2, "bevel2");


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:goo-text",
    "title",       _("Goo on Text"),
    "categories",  "Artistic",
    "reference-hash", "10gz1ak3aa10aavx65421xc254001b2ac",
    "description", _("Put goo on text using GEGL. It is a strong recommendation that both colors are similar or the same "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Goo on Text..."),
    NULL);
}

#endif
