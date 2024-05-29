#include "Pursuit.h"

void Pursuit::Init()
{
	interval = kInterval;
}

bool Pursuit::Execution()
{

	if (interval > 0) {
		interval--;
		return true;
	}
	if (enemys_.empty()) {
		interval = kInterval;
		return false;
	}

	if (interval == 10) {
		// モデルを作る
		models_.reserve(enemys_.size());
		for (int It = 0; It < enemys_.size(); It++) {
			if (enemys_[It]->GetIsDeath() == true) {
				continue;
			}
			models_.emplace_back();
			models_[It].LoadShortPath("cube/cube.obj");
			//models_[It].name = "pursuit";
			models_[It].worldTF.translation = enemys_[It]->GetPosition();
			models_[It].worldTF.translation.y += 1.0f;
		}
		interval--;
		return true;
	}
	else if (interval == 0) {
		interval = kInterval;
		// ダメージを与える
		for (int It = 0; It < enemys_.size(); It++) {
			if (enemys_[It]->GetIsDeath() == true) {
				continue;
			}
			enemys_[It]->DecreaseHP(damage_);
		}
		// 最後にクリアする
		enemys_.clear();
		models_.clear();
		return false;
	}
}
