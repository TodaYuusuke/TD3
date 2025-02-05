#include "Player.h"

// ゲームシーン
#include "Game/Scenes/Game/GameScene.h"

#include "Game/Objects/GameMask.h"

#include "Status/Derived/Idol.h"
#include "Status/Derived/Slash.h"
#include "Status/Derived/Moment.h"
#include "Status/Derived/Damage.h"

using Behavior = IStatus::Behavior;
using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Object::Collider;
float Player::MoveMax = 50.0f;

void Player::Initialize()
{
	// モデル読み込み
	demoModel_.LoadFile("Player/Player.obj");
	demoModel_.isActive = true;
	demoModel_.name = "Player";

	demoModel_.transform.translation.z = -4.0f;

	// 設定の初期化
	config_.Initialize();
	// パラメータの初期化
	parameter_.Initialize(&config_);
	// パラメータを反映させる
	parameter_.ResetParameter();

	// 武器を作成
	weapon_.reset(new Weapon);
	weapon_->Initialize();
	weapon_->SetParent(&demoModel_.transform);
	//weapon_->SetTPointer(&easeT_);

	behavior_ = Behavior::Root;

	SlashPanel::InitParticle();

	// 居合攻撃の UI
	slashPanel_.reset(new SlashPanel);
	slashPanel_->Initialize();

	invincibleTime_ = 0.0f;
	maxInvincibleTime_ = 0.0f;

	// コライダー生成
	CreateCollisions();

	// レベル関係を生成
	level_ = std::make_unique<Level>();
	level_->Initialize(this);

	// 状態作成
	statuses_.clear();

	statuses_.push_back(new Idol);
	//statuses_.push_back(new Move);
	statuses_.push_back(new Slash);
	statuses_.push_back(new Moment);
	statuses_.push_back(new Damage);

	for (size_t i = 0; i < statuses_.size(); i++)
	{
		statuses_[i]->Init(this);
	}


	// 今の状態を設定
	currStatus_ = statuses_[static_cast<size_t>(behavior_)];

	pursuit_ = new Pursuit(this);
	eXLife_ = new EXLife();
	eXLife_->Init(this);

	// 攻撃後確認
	hitCheck_.Initialize(this);


	demoModel_.material.enableLighting = true;

	// ゲームオーバーアニメーション
	gameOverMotion_.Add(&demoModel_.transform.translation, lwp::Vector3{ 0, 5, 0 }, 0, 2.0f, LWP::Utility::Easing::Type::InQuart)
		.Add(&demoModel_.transform.rotation, lwp::Vector3{ 0, 15, 0 }, 0, 2.0f, LWP::Utility::Easing::Type::InQuint);
	// 土飛沫のパーティクル
	InitStaticVariable();

	// 光の柱
	lightPillarMotion_.Add(&lightPillar_.transform.scale, LWP::Math::Vector3{ 1.5f,1.5f,1.5f }, 0, 0.1f)
		.Add(&lightPillar_.transform.scale, LWP::Math::Vector3{ -1.5f,-1.5f,-1.5f }, 0.1f, 0.1f);
	// 出現時の光の柱
	lightPillar_.texture = LWP::Resource::LoadTexture("particle/lightPillar.png");
	lightPillar_.name = "LightPillar";
	lightPillar_.transform.scale = { 1,100,1 };
	lightPillar_.isActive = false;
}

void Player::Update()
{
#ifdef DEMO
	// デバッグ表示
	DebugWindow();
#endif

	//*** プレイヤーの生き死にを判別 ***//

	if (!flag_.isAlive_)
	{
		return;
	}

	// ノックバックフラグをoffにする
	if (isEnemyKnockBack_)
	{
		isEnemyKnockBack_ = false;
	}

	//*** 入力系 ***//

	// 移動入力を受け付ける
	CheckInputMove();

	// 攻撃入力を受け付ける
	CheckInputSlash();

	// 状態を変えるか判別
	CheckBehavior();

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		currStatus_ = statuses_[static_cast<size_t>(behavior_)];
		currStatus_->Reset();
		reqBehavior_ = std::nullopt;
	}
	// 状態の更新
	currStatus_->Update();
	weapon_->Update();
	slashPanel_->Update();

	// パラメータに関係してくるのでパラメータの前
	hitCheck_.Update();

	// ダメージ処理が終わった後なら？
	parameter_.Update();

	if (pursuitFlag)
	{
		pursuitFlag = pursuit_->Execution();
	}
	if (parameter_.Flag.eXLifeFlag)
	{
		eXLifeFlag = eXLife_->Update();
	}

	// 経験値が更新された後かと思ったけど別にプレイヤーの更新が終わった後ならどこでもいい
	level_->Update();

	//*** ここから下はフラグによって管理されている ***//

	colliders_.player_.Create(demoModel_.transform.translation + lwp::Vector3(0.0f, 0.5f, 0.0f));

	// 無敵時間確認
	if (flag_.isInvincible_)
	{
		// 無敵フレームを満たすか判断
		invincibleTime_ += (float)lwp::GetDeltaTime();

		// 自機を点滅させる
		DamageEffect();

		if (maxInvincibleTime_ <= invincibleTime_)
		{
			// 無敵を切る
			flag_.isInvincible_ = false;

			// 自機が消えたままにしないようにする
			demoModel_.isActive = true;
		}
	}
	// 無敵なのかどうか判断
	colliders_.player_.isActive = !flag_.isInvincible_;
}

void Player::IncreaseHP()
{
	parameter_.IncreaseHP();
}

void Player::DecreaseHP()
{
	if (parameter_.DecreaseHP())
	{
		flag_.isAlive_ = false;
		GameTimer::GetInstance()->isEnd_ = true;
		demoModel_.isActive = true;
		flag_.isInvincible_ = false;
		gameOverMotion_.Start();
	}
}

void Player::ApplyUpgrade(const UpgradeParameter& para)
{
	parameter_.ApplyUpgrade(para);
}

bool Player::ClearAnime()
{
	if (ClearYUpMotion.t == 0.0f) {
		lightPillar_.transform.translation = demoModel_.transform.translation;
		lightPillar_.isActive = true;
		lightPillarMotion_.Start();
	}

	if (ClearYUpMotion.t == 1.0f)
	{
		ClearZUpMotion.t = (std::min)(ClearZUpMotion.t + 0.01f, 1.0f);
		demoModel_.transform.scale.x = Lerp(demoModel_.transform.scale.x, 0.0f, ClearZUpMotion.t);
		demoModel_.transform.scale.y = Lerp(demoModel_.transform.scale.y, 0.0f, ClearZUpMotion.t);
		demoModel_.transform.scale.z = Lerp(demoModel_.transform.scale.z, 0.0f, ClearZUpMotion.t);
		if (ClearZUpMotion.t == 1.0f) {
			return true;
		}
	}
	else {
		ClearYUpMotion.t = (std::min)(ClearYUpMotion.t + 0.01f,1.0f);
		demoModel_.transform.translation.y = Lerp(demoModel_.transform.translation.y,5.0f, ClearYUpMotion.t);

		ClearZUpMotion.targetpoint.y = demoModel_.transform.scale.y;
	}

	return false;
}

bool Player::GameOverAnime()
{
	if (gameOverMotion_.isEnd())
	{
		isGameOver_ = true;
		if (gameOverFrame_ == 0)
		{
			deadEffect_(64, demoModel_.transform.translation);
			demoModel_.isActive = false;
			weapon_->SetIsActive(false);
			audio[1]->Play();
		}
	}
	else
	{
		demoModel_.isActive = true;
	}

	if (isGameOver_)
	{
		gameOverFrame_++;
		if (gameOverFrame_ >= 90)
		{
			return true;
		}
	}

	return false;
}

void Player::RegistStatus(IStatus::Behavior request)
{
	commands_.push_back(request);
}

lwp::Vector3 Player::GetVectorTranspose(const lwp::Vector3& vec)
{
	lwp::Vector3 vector = vec * lwp::Matrix4x4::CreateRotateXYZMatrix(pCamera_->pCamera_->transform.rotation);
	vector.y = 0.0f;
	return vector.Normalize();
}

#pragma region CollisionFunc

void Player::CreateCollisions()
{
	CreatePlayerCollision();
	CreateWeaponCollision();
}

void Player::CreatePlayerCollision()
{
	// 当たり判定を設定
	//colliders_.player_ = LWP::Object::Collider::AABB();
	// 武器との当たり判定を取る
	colliders_.player_.Create(demoModel_.transform.translation);
	// マスク
	colliders_.player_.mask.SetBelongFrag(GameMask::Player());
	// 敵または敵の攻撃
	colliders_.player_.mask.SetHitFrag(GameMask::Enemy() | GameMask::EnemyAttack());
	// チョットした後隙
	// 別個で用意した当たった時の関数
	colliders_.player_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {OnCollisionPlayer(data); });

	colliders_.player_.isActive = true;
	flag_.isInvincible_ = false;
	colliders_.player_.name = "Player";
}

void Player::CreateWeaponCollision()
{
	// 武器との当たり判定を取る
	colliders_.weapon_.Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	colliders_.weapon_.radius = config_.Length_.WEAPONCOLLISIONRADIUS_;
	// マスク
	colliders_.weapon_.mask.SetBelongFrag(GameMask::Weapon());
	colliders_.weapon_.mask.SetHitFrag(GameMask::Enemy());
	// 別個で用意した当たった時の関数
	colliders_.weapon_.SetOnCollisionLambda([this](lwp::Collider::HitData data) {
		OnCollisionWeapon(data);
		});
	colliders_.weapon_.isActive = false;
#ifdef DEMO
	colliders_.weapon_.name = "Weapon";
#endif
}

#pragma region OnCollisionFunc

void Player::OnCollisionPlayer(lwp::Collider::HitData& data)
{
	if (data.state == OnCollisionState::Trigger &&
		!flag_.isInvincible_)
	{
		if
			(data.hit->mask.GetBelongFrag() & data.self->mask.GetHitFrag())
		{
			reqBehavior_ = Behavior::Damage;
		}
	}
}

void Player::OnCollisionWeapon(lwp::Collider::HitData& data)
{
	// 攻撃が当たった時のアニメーション？
	data;
}

#pragma endregion

#pragma endregion

#pragma region Effect
void Player::InitStaticVariable()
{
	// 土飛沫
	static LWP::Object::Particle soilSplashParticle_;
	soilSplashParticle_.SetPrimitive<Primitive::Cube>();
	soilSplashParticle_.P()->transform.scale = { 0.0001f,0.0001f, 0.0001f };
	soilSplashParticle_.P()->material.enableLighting = true;
	soilSplashParticle_.P()->commonColor = new Utility::Color(0xCD853FFF);
	soilSplashParticle_.initFunction = [&](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = lwp::Vector3{ 0,1,0 } + primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		// 大きさをランダムにする
		int scale = Utility::GenerateRandamNum<int>(25, 50);
		newData.wtf.scale = { scale / 200.0f, scale / 200.0f, scale / 200.0f };

		// 速度ベクトルを生成
		int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
		// 発射のベクトル
		Math::Vector3 dir{ dir1 / 100.0f + -destinate_.x,dir2 / 100.0f + -destinate_.y, dir3 / 100.0f + -destinate_.z };
		// 係数
		float multiply = Utility::GenerateRandamNum<int>(20, 50) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;

		// パーティクル追加
		return newData;
		};
	soilSplashParticle_.updateFunction = [](Object::ParticleData* data) {
		if (Info::GetDeltaTime() == 0.0f) {
			return false;
		}

		// 経過フレーム追加
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;    // 速度ベクトルを加算
		data->wtf.rotation += data->velocity;    // ついでに回転させとく


		// 20フレーム以降から重力を加算
		if (data->elapsedFrame > 20)
		{
			data->velocity.y += -9.8f / 600.0f;
			// yが0以下になったとき跳ねる
			if (data->wtf.translation.y <= 0.1f)
			{
				data->velocity.y *= -0.5f;
			}
		}
		else
		{
			// 速度ベクトルを弱める
			data->velocity *= 0.9f;
		}

		// ちょっとしたら検証開始

		// 速度が極端に遅くなったら終了フェーズ
		if (data->elapsedFrame > 25 &&
			data->velocity.y <= 0.01f && -0.01f <= data->velocity.y &&
			data->wtf.translation.y <= 0.15f && data->wtf.translation.y >= -0.15f)
		{
			data->velocity = { 0.0f,0.0f,0.0f };
			data->wtf.scale *= 0.9f;
			// もし完全に小さくなったなら終了
			if (data->wtf.scale.x <= 0.001f) { return true; }
		}

		return false;
		};
	soilSplashParticle_.isActive = true;
	soilSplashEffect_ = [&](int i, lwp::Vector3 pos) {
		soilSplashParticle_.P()->transform = pos;
		soilSplashParticle_.Add(i);
		};


	static LWP::Object::Particle deadParticle_;
	deadParticle_.SetPrimitive<Primitive::Cube>();
	deadParticle_.P()->transform.scale = { 0.0001f,0.0001f, 0.0001f };
	deadParticle_.P()->material.enableLighting = true;
	deadParticle_.P()->commonColor = new Utility::Color(0xCD853FFF);
	deadParticle_.initFunction = [&](Primitive::IPrimitive* primitive) {
		Object::ParticleData newData{};
		newData.wtf.translation = lwp::Vector3{ 0,1,0 } + primitive->transform.GetWorldPosition();
		newData.wtf.rotation = primitive->transform.rotation;
		// 大きさをランダムにする
		int scale = Utility::GenerateRandamNum<int>(25, 50);
		newData.wtf.scale = { scale / 200.0f, scale / 200.0f, scale / 200.0f };

		// 速度ベクトルを生成
		int dir1 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir2 = Utility::GenerateRandamNum<int>(-100, 100);
		int dir3 = Utility::GenerateRandamNum<int>(-100, 100);
		// 発射のベクトル
		Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
		// 係数
		float multiply = Utility::GenerateRandamNum<int>(20, 50) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;

		// パーティクル追加
		return newData;
		};
	deadParticle_.updateFunction = [](Object::ParticleData* data) {
		// 経過フレーム追加
		data->elapsedFrame++;

		data->wtf.translation += data->velocity;    // 速度ベクトルを加算
		data->wtf.rotation += data->velocity;    // ついでに回転させとく


		// 20フレーム以降から重力を加算
		if (data->elapsedFrame > 20)
		{
			data->velocity.y += -9.8f / 800.0f;
			// yが0以下になったとき跳ねる
			if (data->wtf.translation.y <= 0.1f)
			{
				data->velocity.y *= -0.5f;
			}
		}
		else
		{
			// 速度ベクトルを弱める
			data->velocity *= 0.9f;
		}

		//return data->elapsedFrame > 100 ? true : false;

		// ちょっとしたら検証開始

		// 速度が極端に遅くなったら終了フェーズ
		if (data->elapsedFrame > 25 &&
			data->velocity.y <= 0.01f && -0.01f <= data->velocity.y &&
			data->wtf.translation.y <= 0.15f && data->wtf.translation.y >= -0.15f)
		{
			data->velocity = { 0.0f,0.0f,0.0f };
			data->wtf.scale *= 0.9f;
			// もし完全に小さくなったなら終了
			if (data->wtf.scale.x <= 0.001f) { return true; }
		}

		return false;
		};
	deadParticle_.isActive = true;
	deadEffect_ = [&](int i, lwp::Vector3 pos) {
		deadParticle_.P()->transform = pos;
		deadParticle_.Add(i);
		};
}
#pragma endregion

void Player::CheckInputMove()
{
	lwp::Vector2 direct{ 0.0f,0.0f };
	if (lwp::Keyboard::GetPress(DIK_W))
	{
		direct.y += 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_S))
	{
		direct.y -= 1.0f;
	}
	if (lwp::Keyboard::GetPress(DIK_A))
	{
		direct.x -= 1.0f;
	}
	else if (lwp::Keyboard::GetPress(DIK_D))
	{
		direct.x += 1.0f;
	}
	direct += LWP::Input::Controller::GetLStick();
	direct = direct.Normalize();

	// そもそも移動入力が無かったらフラグを立てない
	flag_.isInputMove_ = !(direct.x == 0 && direct.y == 0);

	// 移動入力があった時に方向を更新する
		// 方向がゼロにならない保護
	if (flag_.isInputMove_)
	{
		destinate_.x = direct.x;
		destinate_.z = direct.y;
	}
}

void Player::CheckInputSlash()
{
	if (lwp::Keyboard::GetTrigger(DIK_SPACE) ||
		lwp::Pad::GetTrigger(XINPUT_GAMEPAD_A))
	{
		flag_.isInputSlash_ = true;
		pursuitFlag = true;
	}
	else
		flag_.isInputSlash_ = false;
}

void Player::CheckBehavior()
{
	std::optional<Behavior> command = std::nullopt;

	// コマンドを設定
	command = reqBehavior_;

	// 積み重ねたコマンドから実際の行動を決定する
	for (std::list<Behavior>::iterator itr = commands_.begin();
		itr != commands_.end(); ++itr)
	{
		if (command)
		{
			// 優先度が高い方にする
			if (static_cast<uint32_t>(command.value()) <= static_cast<uint32_t>(*itr))
			{
				command = *itr;
			}
		}
		else
		{
			command = *itr;
		}
	}

	// コマンドによって行動変化
	if (command)
	{
		switch (command.value())
		{
		case Behavior::Root:
			reqBehavior_ = Behavior::Root;
			break;
		case Behavior::Slash:
			// 居合に入る条件を記述
			// 最大回数に達していないか
			if (slashData_.relationSlash_ < slashData_.maxRelation_){
				reqBehavior_ = Behavior::Slash;
				slashPanel_->Slash();
				soilSplashEffect_(16, demoModel_.transform.translation);
				audio[0]->Play(soundVolume);
			}
			break;
		case Behavior::Moment:
			reqBehavior_ = Behavior::Moment;
			// 突進が終わるときに敵をノックバックさせる
			if (reqBehavior_ != behavior_)
			{
				if (parameter_.Flag.BlowOffFlag)
				{
					isEnemyKnockBack_ = true;
				}
			}
			break;
		case Behavior::Damage:
			reqBehavior_ = Behavior::Damage;
			StartEnemyKnockBack();
			break;
		}
	}

	// 確認したので中身を消す
	commands_.clear();

}

#pragma region DebugFunc

#if DEMO

void Player::DebugWindow()
{
	ImGui::Begin("PlayerWindow");

	ImGui::Text("How To Controll");
	ImGui::Bullet();
	ImGui::Text("WASD or LStick : MOVE");
	ImGui::Bullet();
	ImGui::Text("SPACE or A  : SLASH");

	ImGui::Separator();

	ImGui::Text("\"Reset\" can Revive!");
	if (ImGui::Button("Reset"))
	{
		invincibleTime_ = 0.0f;
		maxInvincibleTime_ = 0.0f;
		flag_.isAlive_ = true;
		parameter_.ResetParameter();
		parameter_.ApplyUpgrade();
	}
	ImGui::Text("Alive : ");	ImGui::SameLine();
	ImGui::Text(flag_.isAlive_ ? "TRUE" : "FAlSE");
	ImGui::Text("Invincible : ");	ImGui::SameLine();
	ImGui::Text(flag_.isInvincible_ ? "TRUE" : "FAlSE");
	ImGui::Text("HP / MAX");
	ImGui::Bullet();	ImGui::Text("%d / %d(%d)", parameter_.Hp.hp_, parameter_.Hp.maxHp_, config_.Count_.MAXHP_);
	ImGui::Separator();


	ImGui::Text("SlashRelation");
	ImGui::Bullet();	ImGui::Text("Num / Max(Base)");
	ImGui::Bullet();	ImGui::Text("%d / %d(%d)", slashData_.relationSlash_, slashData_.maxRelation_, parameter_.Attack.slashNum_);
	ImGui::Text("INCREMENTMOMENT : %.3f", config_.Time_.MOMENTINCREMENT_);

	ImGui::Separator();

	DebugBehavior();

	ImGui::Begin("PlayerHP");

	if (ImGui::Button("Increase"))
	{
		IncreaseHP();
	}if (ImGui::Button("Decrease"))
	{
		DecreaseHP();
	}if (ImGui::Button("IncreaseMax"))
	{
		parameter_.IncreaseHPMAX();
	}if (ImGui::Button("DecreaseMax"))
	{
		parameter_.DecreaseHPMAX();
	}

	ImGui::End();

	DebugSpeeds();
	//DebugTimes();
	//DebugLengths();
	//DebugCounts();
	//DebugParcentages();

	ImGui::Separator();

	ImGui::End();
}

void Player::DebugBehavior()
{
	if (ImGui::TreeNode("Behavior"))
	{
		ImGui::Text("Behavior : ");
		ImGui::SameLine();
		switch (behavior_)
		{
		case Behavior::Root:
			ImGui::Text("ROOT");
			ImGui::Text("BaseFrame : %.3f", config_.Time_.ROOTBASE_);
			//ImGui::Text("MaxFrame  : %.3f", rootData_.maxTime_);
			break;
			/*case Behavior::Move:
				ImGui::Text("MOVE");
				ImGui::Text("BaseFrame : %.3f", config_.Time_.MOVEBASE_);
				ImGui::Text("MaxFrame  : %.3f", moveData_.maxTime_);
				break;*/
		case Behavior::Slash:
			ImGui::Text("SLASH");
			ImGui::Text("BaseFrame : %.3f", config_.Time_.SLASHBASE_);
			//ImGui::Text("MaxFrame  : %.3f", slashData_.maxTime_);
			break;
		case Behavior::Moment:
			ImGui::Text("MOMENT");
			ImGui::Text("BaseFrame : %.3f", config_.Time_.MOMENTBASE_);
			//ImGui::Text("MaxFrame  : %.3f", momentData_.maxTime_);
			break;
		case Behavior::Damage:
			ImGui::Text("DAMAGE");
			ImGui::Text("BaseFrame : %.3f", config_.Time_.DAMAGEBASE_);
			//ImGui::Text("MaxFrame  : %.3f", damageData_.maxTime_);
			break;
		default:
			break;
		}

		ImGui::TreePop();
		ImGui::Separator();
	}
}

void Player::DebugSpeeds()
{
	if (ImGui::TreeNode("Speed"))
	{
		ImGui::DragFloat("MOVE", &config_.Speed_.MOVE_, 0.001f);
		ImGui::DragFloat("SLASH", &config_.Speed_.SLASH_, 0.001f);
		ImGui::DragFloat("MOMENT", &config_.Speed_.MOMENT_, 0.001f);

		ImGui::TreePop();
		ImGui::Separator();
	}
}


void Player::DebugTimes()
{
	if (ImGui::TreeNode("Time"))
	{
		if (ImGui::TreeNode("Base"))
		{
			ImGui::DragFloat("ROOT", &config_.Time_.ROOTBASE_, 0.001f);
			//ImGui::DragFloat("MOVE", &config_.Time_.MOVEBASE_, 0.001f);
			ImGui::DragFloat("SLASH", &config_.Time_.SLASHBASE_, 0.001f);
			ImGui::DragFloat("MOMENT", &config_.Time_.MOMENTBASE_, 0.001f);
			ImGui::DragFloat("DAMAGE", &config_.Time_.DAMAGEBASE_, 0.001f);

			ImGui::TreePop();
			ImGui::Separator();
		}
		/*if (ImGui::TreeNode("ROOT"))
		{
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("MOVE"))
		{
			ImGui::TreePop();
			ImGui::Separator();
		}*/
		if (ImGui::TreeNode("SLASH"))
		{
			ImGui::DragFloat("JUSTTAKE", &config_.Time_.JUSTTAKETIME_, 0.001f, 0.0f, config_.Time_.SLASHBASE_);
			ImGui::DragFloat("JUSTINVINCIGLECORRECTION", &config_.Time_.SLASHENDINVINCIBLETIME_, 0.001f, 0.0f, config_.Time_.SLASHBASE_ - config_.Time_.JUSTTAKETIME_);
			ImGui::DragFloat("JUSTINVINCIGLEADD", &config_.Time_.JUSTINVINCIBLEADD_, 0.001f, 0.0f);

			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("MOMENT"))
		{
			ImGui::DragFloat("INCREMENT", &config_.Time_.MOMENTINCREMENT_, 0.001f);

			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("DAMAGE"))
		{
			ImGui::DragFloat("INVINCIBLE", &config_.Time_.DAMAGEINVINCIBLE_, 0.001f);

			ImGui::TreePop();
			ImGui::Separator();
		}

		ImGui::TreePop();
		ImGui::Separator();
	}
}

void Player::DebugLengths()
{
	if (ImGui::TreeNode("Length"))
	{
		ImGui::DragFloat("WEAPONRADIUS", &config_.Length_.WEAPONCOLLISIONRADIUS_, 0.01f);
		ImGui::DragFloat("WEAPONCORRECTION", &config_.Length_.WEAPONPLUSCORRECTION_, 0.01f);
		ImGui::DragFloat("JUSTRADIUS", &config_.Length_.JUSTCOLLISIONRADIUS_, 0.01f);


		ImGui::TreePop();
		ImGui::Separator();
	}
}
void Player::DebugCounts()
{
	if (ImGui::TreeNode("Count"))
	{
		int i = config_.Count_.SLASHRELATIONBASE_;
		// 入力されたら
		if (ImGui::DragInt("SLASHRELATION", &i, 1, 10))
		{
			config_.Count_.SLASHRELATIONBASE_ = i;
		}
		int j = config_.Count_.BASEHP_;
		// 入力されたら
		if (ImGui::DragInt("BASEHP", &j, 1, 10))
		{
			config_.Count_.BASEHP_ = j;
		}
		int k = config_.Count_.MAXHP_;
		// 入力されたら
		if (ImGui::DragInt("MAXHP", &k, 1, 10))
		{
			config_.Count_.MAXHP_ = j;
		}

		ImGui::TreePop();
		ImGui::Separator();
	}
}

void Player::DebugParcentages()
{
	if (ImGui::TreeNode("Parcentage"))
	{
		ImGui::SliderFloat("JUSTENABLE", &config_.Parcent_.JUSTENABLE_, 0.0f, 1.0f);

		ImGui::TreePop();
		ImGui::Separator();
	}
}

#endif

#pragma endregion

void Player::DamageEffect()
{
	int invincibleTime = int(invincibleTime_ * 1000);
	if (invincibleTime % 3 == 0)
	{
		demoModel_.isActive = false;
	}
	else
	{
		demoModel_.isActive = true;
	}
}

std::function<void(int, lwp::Vector3)> Player::deadEffect_ = nullptr;