#include "HitCheck.h"
#include <Game/Objects/Enemy/IEnemy/IEnemy.h>
#include <Game/Objects/Player/Player.h>

void L::HitCheck::Initialize()
{
	hitList_.clear();
	hitEnemys_.clear();
	destroyEnemyNum_ = 0u;
	postDestroyEnemyNum_ = 0u;
}

void L::HitCheck::Update(Player* const player)
{
	postDestroyEnemyNum_ = destroyEnemyNum_;
	// 消しながら更新
	for (std::list<HitEnemyTime*>::iterator itr = hitList_.begin();
		itr != hitList_.end();)
	{
		// nullptr か既に死んでいたら
		// 一定時間経っていたら
		if (CheckDeleteTarget((*itr)))
		{
			// 削除
			itr = hitList_.erase(itr);
			// 倒した瞬間で判断させれば良い
			CheckPenetrate(player);
			continue;
		}
		// いろんなフラグを見て更新
		CheckFlags((*itr), player);
		// 時間プラス
		(*itr)->hitTime += lwp::GetDeltaTimeF();

		// 次へ
		itr++;
	}


#ifdef DEMO

	ImGui::Begin("HitCheck");

	ImGui::Text("%d -> %d", postDestroyEnemyNum_, destroyEnemyNum_);
	ImGui::Text("Hit    : %d", hitList_.size());
	ImGui::Text("Enemys : %d", hitEnemys_.size());

	ImGui::End();

#endif // DEMO
}

void L::HitCheck::AddHitEnemy(IEnemy* enemy)
{
	hitList_.push_back(new HitEnemyTime(enemy));
	// 登録されていないのなら
	if (!hitEnemys_.contains(enemy->GetIndex()))
	{
		// 生きていることを登録して添え字加算
		hitEnemys_[enemy->GetIndex()] = true;
		// ここでトリガーとして設定できる

	}
}

bool L::HitCheck::CheckDeleteTarget(HitEnemyTime* het)
{
	// 安全用
	if (!het)
	{
		return true;
	}
	if (het->target->GetIsDeath())
	{
		// 倒したので加算
		if (hitEnemys_.contains(het->target->GetIndex()) && hitEnemys_[het->target->GetIndex()])
		{
			destroyEnemyNum_++;
			hitEnemys_[het->target->GetIndex()] = false;
			// 次まで使わないので削除
			hitEnemys_.erase(het->target->GetIndex());
		}
		return true;
	}
	// ここで判定が終わる
	if (kMaxTime <= het->hitTime)
	{
		// ここでリリースとして設定できる
		return true;
	}
	return false;
}

void L::HitCheck::CheckFlags(HitEnemyTime* het, Player* const player)
{
	// ここは常に判定できる
	
	// 倒した敵が出てきたときに判定しているが、一応ここでも判定している
	// どうやったって回復できるのは 1 フレームのみ
	CheckPenetrate(player);
}

void L::HitCheck::CheckPenetrate(Player* const player)
{
	// ここでアップグレードのフラグを使いたい
	// HP 回復出来ますか？
	if (player->parameter_.Flag.penetrationFlag)
	{
		// 一定以上かつ前のフレームと一緒ではない
		// 一定値以上になった瞬間をコールバックさせたい
		if (destroyEnemyNum_ != postDestroyEnemyNum_ &&
			destroyEnemyNum_ % player->config_.Count_.UPGRADEPENETORATIONNUM_ == 0)
		{
			// HP 回復を実行できる状態にする
			player->parameter_.Flag.isActiveIncreaseHP = true;
		}
	}
}

// このクラスを作るに至ったメモ書き

// 個別で機能するものを更新する
// もう全部フラグ追加すればいいと思う
// プレイヤーにマネージャーを渡して敵を登録する
// 攻撃がヒットしたという情報のみを格納するマネージャー
// フラグに応じてどういった操作をするか決定する
// 新規クラス？
// このクラスが持つ必要ない？
// アップグレード内容参照する必要
// プレイヤーから参照？
// 個別で参照できるようにする？