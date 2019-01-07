#include "hr.h"

int main()
{
	HR tg;
	if (!tg.init()) return 0;

	//cubemap�𐶐�
	auto tex = tg.texture_manager.addTexture(
		"texture\\cubemap\\lagoon_ft.tga",
		"texture\\cubemap\\lagoon_bk.tga",
		"texture\\cubemap\\lagoon_dn.tga",
		"texture\\cubemap\\lagoon_up.tga",
		"texture\\cubemap\\lagoon_rt.tga",
		"texture\\cubemap\\lagoon_lf.tga",
		CubeMap);
	tg.skybox.setTex(tex);

	//shader�𐶐�
	tg.shader_manager.addShader<VolumeShader>("vs", "volume.vert", "volume.frag");
	tg.shader_manager.addShader<HairShader>("hs", "geom_hair.vert", "marschener.frag","cylinder_hair.geom");

	//computeshader�𐶐�
	FluidShader fcs;
	fcs.createComputeShader("JS_add_data.comp", "JS_diffuse.comp", "JS_advect.comp", "JS_divergence.comp","JS_project.comp", "JS_projvel.comp", "JS_set_bnd.comp");

	HairFTLShader hcs;
	hcs.createComputeShader("hair_FTL.comp","hair_collision.comp");

	//light�𐶐�Point
	auto &lig(tg.light_manager.addLight("light1", Point));
	lig.setPosition(0.0f, 10.0f, 0.0f);
	lig.setVector(0.0f, -1.0f, 1.0f);
	lig.setColor(1.0f, 1.0f, 1.0f);

	auto &mat1(tg.material_manager.addMaterial("hair_material"));
	auto &mat2(tg.material_manager.addMaterial("volume_material"));

	//���̂𐶐�
	Fluid3D fluid0;
	fluid0.loadObject3D(32);
	fluid0.OffCompuress();
	
	//�є��𐶐�
	auto &hair0(tg.object3d_manager.addObject3D<Basic3D>("hair0",SphereHair,SHair,5000,15,0.5f,0.2f));
	hair0.setMaterial(mat1);
	hair0.setPosition(0.0f,1.0f,0.0f);
	//�{�����[���f�[�^�𐶐�
	auto &volume0(tg.object3d_manager.addObject3D<Volume3D>("volume0", fluid0.getSimulate_tex()[2], SVolume, 200.0f, 0.1f,fluid0.getCell_size()));
	volume0.setMaterial(mat2);

	//�����_�����O
	while (tg.shouldClose() == GL_FALSE)
	{
		//compute shader
		fcs.add_data(fluid0);
		fcs.update(fluid0, 1.0f / 30.0f);

		hcs.update(hair0,fluid0);

		//draw shader
		tg.shader_manager.getShader<HairShader>("hs").draw(hair0, lig, tg.camera);
		tg.shader_manager.getShader<VolumeShader>("vs").draw(volume0, lig, tg.camera);

		tg.object_move(hair0);
		tg.update();
	}

	tg.shutdown();
	return 0;
}