#include"manager.h"

void LightManager::showResource()
{
	if (ImGui::TreeNode("lights")) {
		for (auto &x : lights)
		{
			if (ImGui::TreeNode(x.first)) {
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 50), ImGuiWindowFlags_NoTitleBar);
				const char *type = checkLightType(x.second->getType());
				ImGui::Text("Type %s", type);
				float pos[3];
				deepcopy_vec3(x.second->getPosition(), pos);
				ImGui::SliderFloat3("position", pos, -10.0f, 10.0f);
				x.second->setPosition(pos[0], pos[1], pos[2]);
				ImGui::EndChild();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void Object3DManager::showResource()
{
	if (ImGui::TreeNode("object3ds")) {
		for (auto &x : object3ds)
		{
			if (ImGui::TreeNode(x.first)) {
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 100), ImGuiWindowFlags_NoTitleBar);
				const char *type = checkBasic3DType(x.second->getType());
				ImGui::Text("Type %s", type);
				float pos[3], rot[3], sca[3];
				deepcopy_vec3(x.second->getPosition(), pos);
				deepcopy_vec3(x.second->getRotation(), rot);
				deepcopy_vec3(x.second->getScale(), sca);
				ImGui::SliderFloat3("position", pos, -10.0f, 10.0f);
				ImGui::SliderFloat3("rotation", rot, -180.0f, 180.0f);
				ImGui::SliderFloat3("scale", sca, -10.0f, 10.0f);
				x.second->setPosition(pos[0], pos[1], pos[2]);
				x.second->setRotation(rot[0], rot[1], rot[2]);
				x.second->setScale(sca[0], sca[1], sca[2]);
				if (x.second->getType() == SphereHair || x.second->getType() == PlaneHair)
				{
					ImGui::SliderFloat("hlength", &x.second->getParameter()[2], 0.0f, 1.5f);
					ImGui::SliderFloat("hardness1", &x.second->getParameter()[6], 0.0f, 2.0f);
					ImGui::SliderFloat("m", &x.second->getParameter()[7], 0.0f, 10.0f);
					ImGui::SliderFloat("adhesion", &x.second->getParameter()[8], -2.0f, 2.0f);
					ImGui::SliderFloat("beta_n", &x.second->getParameter()[10], 0.0f, 1.0f);
					ImGui::SliderFloat("beta_m", &x.second->getParameter()[11], 0.0f, 1.0f);
					ImGui::SliderFloat("alpha", &x.second->getParameter()[12], 0.0f, 1.0f);
					ImGui::SliderFloat("sigma", &x.second->getParameter()[13], 0.0f, 1.0f);
					ImGui::SliderFloat("weight", &x.second->getParameter()[9], 0.001f, 0.1f);
					int angle = int(x.second->getParameter()[14]); ImGui::SliderInt("corners", &angle, 2, 8);
					x.second->getParameter()[14] = float(angle);
					ImGui::SliderFloat("sharp", &x.second->getParameter()[15], 0.001f, 1.0f);
					x.second->getParameter()[5] = x.second->getParameter()[2] + x.second->getParameter()[3];
				}
				else if (x.second->getType() == Volume || x.second->getType() == VCube || x.second->getType() == VRandom)
				{
					ImGui::SliderFloat("threshold", &x.second->getParameter()[0], 0.0f, 1.0f);
					int s = int(x.second->getParameter()[1]);
					ImGui::SliderInt("slices", &s, 0, 500);
					x.second->getParameter()[1] = float(s);
				}
				ImGui::EndChild();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void TextureManager::showResource()
{
	if (ImGui::TreeNode("textures")) {
		for (auto &x : textures)
		{
			if (ImGui::TreeNode(x.first)) {
				ImGui::Text("tex %d", x.second);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void MaterialManager::showResource()
{
	if (ImGui::TreeNode("materials")) {
		for (auto &x : materials)
		{
			if (ImGui::TreeNode(x.first)) {
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 100), ImGuiWindowFlags_NoTitleBar);
				float d[3], s[3], a[3],alpha,rough;
				deepcopy_vec3(x.second->getKDiff(), d);
				deepcopy_vec3(x.second->getKSpec(), s);
				deepcopy_vec3(x.second->getKAmb(), a);
				alpha = x.second->getTransparency();
				rough = x.second->getRoughness();
				ImGui::ColorEdit3("kdiff", d, 0);
				ImGui::ColorEdit3("kspec", s, 0);
				ImGui::ColorEdit3("kamb", a, 0);
				ImGui::SliderFloat("transparency", &alpha, 0.0f, 1.0f);
				ImGui::SliderFloat("roughness", &rough, 0.0f, 1.0f);
				x.second->setkDiff(d[0], d[1], d[2]);
				x.second->setKSpec(s[0], s[1], s[2]);
				x.second->setKAmb(a[0], a[1], a[2]);
				x.second->setTransparency(alpha);
				x.second->setRoughness(rough);
				ImGui::EndChild();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void  ShaderManager::showResource()
{
	if (ImGui::TreeNode("shaders")) {
		for (auto &x : shaders)
		{
			if (ImGui::TreeNode(x.first)) {
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}