#include "Pursuit.h"

void Pursuit::Init()
{
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
			models_.emplace_back();
			models_[It].LoadFile("cube/cube.obj");
			models_[It].name = "pursuit";
			models_[It].transform.translation = enemys_[It]->GetPosition();
		}
		interval--;
		return true;
	}
	else if (interval < 0) {
		interval = kInterval;
		// ダメージを与える
		for (int It = 0; It < enemys_.size(); It++) {
			enemys_[It]->DecreaseHP(damage_);
		}
		// 最後にクリアする
		enemys_.clear();
		models_.clear();
		aabb_.clear();
		return false;
	}
	else {
		interval--;
		return true;
	}
}
