#include "HitCheck.h"
#include <Game/Objects/Enemy/IEnemy/IEnemy.h>
#include <Game/Objects/Player/Player.h>

#include <Game/Objects/Upgrade/Function/FuncList.h>

void L::HitCheck::Initialize(Player* const player)
{
	player_ = player;
	hitList_.clear();
	hitEnemys_.clear();
	destroyEnemyNum_ = 0u;
	postDestroyEnemyNum_ = 0u;
	functions_.clear();

	// 機能を登録する
	functions_.push_back(new Penetration(&destroyEnemyNum_, &postDestroyEnemyNum_));
	functions_.push_back(new Burning);
}

void L::HitCheck::Update()
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
			// 倒した瞬間で判断させれば良い
			//CheckPenetrate(player_);
			// 機能ごとに更新
			for (IFunction* func : functions_)
			{
				func->SlayEnemy(*itr, player_);
			}
			// 削除
			itr = hitList_.erase(itr);
			continue;
		}
		// 機能ごとに更新
		for (IFunction* func : functions_)
		{
			func->Update(*itr, player_);
		}

		// いろんなフラグを見て更新
		//CheckFlags((*itr), player_);

		// 時間プラス
		(*itr)->hitTime += lwp::GetDeltaTimeF();
		// 次へ
		itr++;
	}

	// 前か後ろどっちがいいかはわからない
	// フラグを元に戻す
	for (std::list<HitEnemyTime*>::iterator itr = hitList_.begin();
		itr != hitList_.end(); ++itr)
	{
		(*itr)->wasBurning = false;
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
	// 登録されていないのなら
	if (!hitEnemys_.contains(enemy->GetIndex()))
	{
		// 生きていることを登録して添え字加算
		hitEnemys_[enemy->GetIndex()] = true;
		// ここでトリガーとして設定できる
		hitList_.push_back(new HitEnemyTime(enemy));
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
//
//void L::HitCheck::CheckFlags(HitEnemyTime* het, Player* const player)
//{
//	// ここは常に判定できる
//	het;
//	// 倒した敵が出てきたときに判定しているが、一応ここでも判定している
//	// どうやったって回復できるのは 1 フレームのみ
//	CheckPenetrate(player);
//}
//
//void L::HitCheck::CheckPenetrate(Player* const player)
//{
//	// ここでアップグレードのフラグを使いたい
//	// HP 回復出来ますか？
//	if (player->parameter_.Flag.penetrationFlag)
//	{
//		// 一定以上かつ前のフレームと一緒ではない
//		// 一定値以上になった瞬間をコールバックさせたい
//		if (destroyEnemyNum_ != postDestroyEnemyNum_ &&
//			destroyEnemyNum_ % player->config_.Count_.UPGRADEPENETORATIONNUM_ == 0)
//		{
//			// HP 回復を実行できる状態にする
//			player->parameter_.Flag.isActiveIncreaseHP = true;
//		}
//	}
//}

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