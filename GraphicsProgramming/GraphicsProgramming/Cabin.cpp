#include "Cabin.h"
Cabin::Cabin() {
	cabin_stone_base.load(        "models/cabin/1_Cabin_Stone_Base.obj",          "models/cabin/textures/rocks/rocksDESERT.png");

	 cabin_wood_steps.load(       "models/cabin/2_Cabin_Wood_Steps.obj",          "models/cabin/textures/wood/T_brightwood_basecolorORANGE.png");

	 cabin_wood_door.load(        "models/cabin/3_Cabin_Wood_Door.obj",           "models/cabin/textures/wood/T_brightwood_basecolorORANGE.png");
	  
	 cabin_wood_floor.load(       "models/cabin/4_Cabin_Wood_Floor.obj",          "models/cabin/textures/wood/T_brightwood_basecolorORANGE.png");

	 cabin_wood_frame.load(       "models/cabin/5_Cabin_Wood_Frame.obj",          "models/cabin/textures/wood/T_brightwood_basecolorORANGE.png");

	 cabin_wood_walls.load(       "models/cabin/6_Cabin_Wood_Walls.obj",          "models/cabin/textures/wood/T_brightwood_basecolorORANGE.png");

	 cabin_wood_roofframe.load(   "models/cabin/7_Cabin_Wood_RoofFrame.obj",      "models/cabin/textures/wood/T_brightwood_basecolorORANGE.png");

	 cabin_roof_roof.load(        "models/cabin/8_Cabin_Wood_Roof.obj",           "models/cabin/textures/rooftiles/T_darkwood_basecolorBRIGHT.png");

	 cabin_roof_roofplanks.load(  "models/cabin/9_Cabin_Wood_RoofPlanks.obj",     "models/cabin/textures/rooftiles/T_darkwood_basecolorBRIGHT.png");

	 cabin_wood_crossplanks.load( "models/cabin/10_Cabin_Wood_CrossPlanks.obj",   "models/cabin/textures/wood/T_brightwood_basecolorDARK.png");
}
