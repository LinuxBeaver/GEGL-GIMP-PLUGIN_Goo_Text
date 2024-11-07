
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

property_enum(guichange, _("Show Legacy features"),
    guiendgoo, guichangegoo,
    GOO_SHOW_DEFAULT)
  description(_("Change the GUI option"))


enum_start (guichangegoo)
enum_value   (GOO_SHOW_DEFAULT, "defaultgoo", N_("Current Sliders"))
enum_value   (GOO_SHOW_ALL, "legacygoo", N_("Legacy Sliders"))
  enum_end (guiendgoo)


enum_start (goo_blend)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "colordodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "grainmerge",
              N_("Grain Merge"))
enum_end (gooblend)

property_enum (blendmode, _("Blend Mode of Dripping Goo"),
    gooblend, goo_blend,
    GEGL_BLEND_MODE_TYPE_MULTIPLY)

property_color (color, _("Color of dripping goo"), "#00f73b")
    description (_("The color to paint over the input"))

property_double (tile_size, _("Coat text with dripping goo"), 6.0)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (3.40, 15.0)
    ui_meta     ("unit", "pixel-distance")

property_double (tile_saturation, _("Dripping goo spread"), 2.0)
    description (_("Expand tiles by this amount"))
    value_range (2, 2.50)


property_seed (seed, _("Random seed for dripping goo"), rand)


property_int (strength, _("Length of Goo drip"), 35)
 description (_("Higher values increase the magnitude of the effect"))
 value_range (1, 500)

property_int  (median, _("Radius of drip"), 3)
  value_range (3, 6)
  ui_range    (3, 6)
  ui_meta     ("unit", "pixel-distance")
  description (_("Median Blur Radius - Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_double (bevel, _("Bevel depth dripping Goo"), 15.0)
  value_range (0.0, 100.0)
  ui_range (0.0, 100.0)
  ui_gamma (1.5)

property_double (bevelelevation, _("Bevel elevation dripping Goo"), 90.0)
  value_range (0.0, 180.0)
  ui_range (0.0, 180.0)
  ui_gamma (1.5)


property_double (alpha, _("Threshold Alpha"), 0.09)
    value_range (0.09, 0.1)  
    ui_range    (0.09, 0.1)
    description(_("Threshold Alpha's Scalar threshold level (overridden if an auxiliary input buffer is provided.)."))
    ui_meta     ("role", "output-extent")



property_color (color2, _("Goo on top color"), "#00f73b")
    description (_("The color to paint on the top goo"))
ui_meta ("visible", "guichange {legacygoo}")

property_double (tile_size2, _("Goo on top size"), 5.2)
    description (_("Internal Cubism Tile Size for the goo on top"))
    value_range (4.8, 8.0)
    ui_meta     ("unit", "pixel-distance")
ui_meta ("visible", "guichange {legacygoo}")

property_double (tile_saturation2, _("Goo on top spread"), 1.0)
    description (_("Expand cubism tiles by this amount"))
    value_range (0.8, 1.0)
ui_meta ("visible", "guichange {legacygoo}")

property_seed (seed2, _("Random seed for goo on top"), rand2)
ui_meta ("visible", "guichange {legacygoo}")

property_int  (median2, _("Radius for goo on top"), 3)
  value_range (3, 6)
  ui_range    (3, 6)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))
ui_meta ("visible", "guichange {legacygoo}")

property_int (box2, _("Smooth goo on top"), 2)
   description(_("Radius of square pixel region, (width and height will be radius*2+1)"))
   value_range (1, 2)
   ui_range    (1, 2)
   ui_gamma   (1.5)
ui_meta ("visible", "guichange {legacygoo}")

property_double (bevel2, _("Bevel Goo on top"), 15.0)
  value_range (0.0, 100.0)
  ui_range (0.0, 100.0)
  ui_gamma (1.5)
ui_meta ("visible", "guichange {legacygoo}")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     goo
#define GEGL_OP_C_SOURCE goo.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *output;
 GeglNode *color;
 GeglNode *color2;
 GeglNode *atop;
 GeglNode *over;
 GeglNode *cubism;
 GeglNode *median;
 GeglNode *median2;
 GeglNode *multiply;
 GeglNode *multiply2;
 GeglNode *cubism2;
 GeglNode *wind2;
 GeglNode *alpha2;
 GeglNode *box;
 GeglNode *box2;
 GeglNode *light;
 GeglNode *alpha;
 GeglNode *wind;
 GeglNode *bevel;
 GeglNode *bevel2;
 GeglNode *overlay;
 GeglNode *hardlight;
 GeglNode *grainmerge;
 GeglNode *colordodge;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);


  State *state = o->user_data = g_malloc0 (sizeof (State));


  state->input    = gegl_node_get_input_proxy (gegl, "input");
    state->output   = gegl_node_get_output_proxy (gegl, "output");

    state->color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

    state->color2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

    state->over    = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);

    state->atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-atop",
                                  NULL);


    state->median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

    state->median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

    state->cubism    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);

    state->cubism2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);


    state->wind    = gegl_node_new_child (gegl,
                                  "operation", "lb:zzwind", "direction", 3,
                                  NULL);

/* This is a hidden operation of mine that allows gegl:wind to go up to 500. Where in default it is locked at 100. This is
one of the few hidden operations that still remain. The direction number is calling the wind to go down */

    state->wind2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:wind",
                                  NULL);

    state->alpha    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

    state->alpha2    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

    state->box    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur", "radius", 2,
                                  NULL);

    state->box2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);
    state->light = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

    state->bevel = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "bevel1", 90.0,
                                  NULL);

    state->bevel2 = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "bevel1", 90.0,
                                  NULL);

    state->multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

    state->multiply2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

    state->hardlight = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);
                           

    state->colordodge = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 42, "composite-mode", 0, NULL);
                             



state->grainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);


state->overlay = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);


}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


  GeglNode *multiply = state->multiply; 
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: multiply = state->multiply; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGE: multiply = state->colordodge; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAY: multiply = state->overlay; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: multiply = state->hardlight; break;;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: multiply = state->grainmerge; break;
default: multiply = state->multiply;
}



gegl_node_link_many (state->input, state->atop, state->over, state->output, NULL);
gegl_node_link_many (state->input, state->color, state->cubism, state->wind, state->median, state->alpha, multiply, state->box, NULL);
gegl_node_connect (multiply, "aux", state->bevel, "output");
gegl_node_connect (state->multiply2, "aux", state->bevel2, "output");
gegl_node_link_many (state->alpha, state->bevel, NULL);
gegl_node_link_many (state->alpha2, state->bevel2, NULL);
gegl_node_link_many (state->atop, state->color2, state->light, state->cubism2, state->wind2, state->median2, state->alpha2, state->multiply2, state->box2, NULL);
gegl_node_connect (state->atop, "aux", state->box, "output");
gegl_node_connect (state->over, "aux", state->box2, "output");


  gegl_operation_meta_redirect (operation, "color", state->color, "value");
  gegl_operation_meta_redirect (operation, "color2", state->color2, "value");
  gegl_operation_meta_redirect (operation, "seed", state->cubism, "seed");
  gegl_operation_meta_redirect (operation, "seed2", state->cubism2, "seed");
  gegl_operation_meta_redirect (operation, "tile_size2", state->cubism2, "tile-size");
  gegl_operation_meta_redirect (operation, "tile_size", state->cubism, "tile-size");
  gegl_operation_meta_redirect (operation, "tile_saturation", state->cubism, "tile-saturation");
  gegl_operation_meta_redirect (operation, "tile_saturation2", state->cubism2, "tile-saturation");
  gegl_operation_meta_redirect (operation, "strength", state->wind, "strength");
  gegl_operation_meta_redirect (operation, "median", state->median, "radius");
  gegl_operation_meta_redirect (operation, "median2", state->median2, "radius");
  gegl_operation_meta_redirect (operation, "alpha", state->alpha, "value");
  gegl_operation_meta_redirect (operation, "box2", state->box2, "radius");
  gegl_operation_meta_redirect (operation, "bevel", state->bevel, "bevel2");
  gegl_operation_meta_redirect (operation, "bevelelevation", state->bevel, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel2", state->bevel2, "bevel2");


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

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
