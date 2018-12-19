#include "hr.h"

int main()
{
	HR tg;
	if (!tg.init()) return 0;

	//shader‚ğ¶¬
	tg.shader_manager.addShader<LineShader>("ls", "point.vert", "point.frag");
	tg.shader_manager.addShader<HairShader>("hs", "geom_hair.vert", "marschener.frag", "cylinder_hair.geom");
	tg.shader_manager.addShader<MeshShader>("ms", "mesh.vert", "cook_torrance_brdf.frag");
	tg.shader_manager.addShader<VolumeShader>("vs", "volume.vert", "volume.frag");

	//computeshader‚ğ¶¬
	HairFTLShader hcs;
	hcs.createComputeShader("hair_FTL.comp", "hair_collision.comp");

	//light‚ğ¶¬Point
	auto &lig(tg.light_manager.addLight("light1", Point));
	lig.setPosition(0.0f, 10.0f, 0.0f);
	lig.setVector(0.0f, -1.0f, 1.0f);
	lig.setColor(1.0f, 1.0f, 1.0f);

	
	//cubemap‚ğ¶¬
	auto tex = tg.texture_manager.addTexture(
		"texture\\cubemap\\lagoon_ft.tga",
		"texture\\cubemap\\lagoon_bk.tga",
		"texture\\cubemap\\lagoon_dn.tga",
		"texture\\cubemap\\lagoon_up.tga",
		"texture\\cubemap\\lagoon_rt.tga",
		"texture\\cubemap\\lagoon_lf.tga",
	CubeMap);
	tg.skybox.setTex(tex);
	

	auto tex1 = tg.texture_manager.addTexture("texture\\hair_test.tga",ColorMap);

	//material‚ğ¶¬
	auto &mat(tg.material_manager.addMaterial("hair1_material"));

	auto &mat2(tg.material_manager.addMaterial("volume_material"));
	mat2.setCubeMap(tex, "lagoon");
	
	//object‚ğ¶¬
	auto &obj(tg.object3d_manager.addObject3D<Basic3D>("hair1", SphereHair,SHair,10000.0f, 15.0f, 1.5f, 0.5f));
	obj.setMaterial(mat);
	
	
	auto &obj2(tg.object3d_manager.addObject3D<Basic3D>("sphere1", Sphere,SMesh));
	obj2.setMaterial(mat);
	obj2.setScale(0.5f,0.5f,0.5f);
	obj2.setPosition(0.0, 0.0, 0.0);
	
	auto &obj3(tg.object3d_manager.addObject3D<Volume3D>("volume0",obj.space_tex, SVolume,10.0f, 0.001f, obj.getParameter()[4]));
	obj3.setMaterial(mat2);

	//ƒŒƒ“ƒ_ƒŠƒ“ƒO
	while (tg.shouldClose() == GL_FALSE)
	{
		//compute shader
		hcs.update(obj,obj2.getPosition());

		//draw shader
		//tg.shader_manager.getShader<VolumeShader>("vs").draw(obj3, lig, tg.camera);
		tg.shader_manager.getShader<MeshShader>("ms").draw(obj2, lig, tg.camera);
		tg.shader_manager.getShader<HairShader>("hs").draw(obj, lig, tg.camera);
		
		//tg update
		//Wtg.object_move(obj2);

		float size = obj.getParameter()[5] * 1.75f;
		obj3.setScale(size, size, size);

		tg.update();
	}

	tg.shutdown();
	return 0;
}