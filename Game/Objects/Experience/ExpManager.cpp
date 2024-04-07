#include "ExpManager.h"

using namespace LWP;
using namespace LWP::Object;
using namespace LWP::Math;

ExpManager::~ExpManager()
{
	// すべてを消す
	for (std::list<Experience*>::iterator itr = exps_.begin(); itr != exps_.end();)
	{
		// 消す
		delete* itr;
		itr = exps_.erase(itr);
	}
}

void ExpManager::Initialize()
{
	// リストの初期化
	exps_.clear();
}

void ExpManager::Update()
{
	// 消しながら更新
	for (std::list<Experience*>::iterator itr = exps_.begin(); itr != exps_.end();)
	{
		// 消えているなら
		if ((*itr)->isDead_)
		{
			delete* itr;
			itr = exps_.erase(itr);
			continue;
		}
		// 更新
		(*itr)->Update();

		// 次へ
		++itr;
	}

#ifdef DEMO

	DebugWindow();

#endif // DEMO


}

void ExpManager::Create(const lwp::Vector3& pos)
{
	Experience* data = new Experience(pos);
	data->Update();
	exps_.push_back(data);
}
#ifdef DEMO
void ExpManager::DebugWindow()
{
	static Experience* temp = nullptr;
	static Vector3 pos = { 0.0f,0.0f,0.0f };

	ImGui::Begin("Levels");

	//if (ImGui::BeginTabBar("exp"))
	//{

	//	if (ImGui::BeginTabItem("ExpManager"))
	//	{
	if (ImGui::TreeNode("EXP"))
	{
		ImGui::DragFloat3("pso", &pos.x, 0.1f);

		ImGui::Separator();

		if (ImGui::Button("Create"))
		{
			Create(pos);
		}

		ImGui::Text(temp ? "IN" : "NULL");
		if (ImGui::Button("Delete") && temp)
		{
			temp->isDead_ = true;
			temp = nullptr;
		}

		ImGui::Separator();

		ImGui::BeginChild(ImGui::GetID((void*)0),ImVec2(0,100));

		std::list<Experience*>::iterator itr = exps_.begin();
		for (size_t i = 0; i < exps_.size(); i++)
		{
			if (ImGui::Button(("Exp" + std::to_string(i)).c_str()))
			{
				temp = (*itr);
			}
			//ImGui::Separator();
			++itr;
		}

		ImGui::EndChild();
		ImGui::Separator();

		ImGui::TreePop();
	}
	//		ImGui::EndTabItem();
	//	}

	//	ImGui::EndTabBar();
	//}

	ImGui::End();
}
#endif //DEMO
