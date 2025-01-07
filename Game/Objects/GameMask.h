#pragma once

#include <Adapter.h>

namespace GameMask
{
	/// <summary>
	/// プレイヤー自身
	/// </summary>
	/// <returns>0b1</returns>
	constexpr uint32_t Player() { return ColMask0; }
	/// <summary>
	/// 敵自身
	/// </summary>
	/// <returns>0b10</returns>
	constexpr uint32_t Enemy() { return ColMask1; }
	/// <summary>
	/// 敵の攻撃
	/// </summary>
	/// <returns>0b100</returns>
	constexpr uint32_t EnemyAttack() { return ColMask2; }
	/// <summary>
	/// プレイヤーの攻撃
	/// </summary>
	/// <returns>0b1000</returns>
	constexpr uint32_t Weapon() { return ColMask3; }
	/// <summary>
	/// 経験値自身
	/// </summary>
	/// <returns>0b10000</returns>
	constexpr uint32_t Exp() { return ColMask4; }
	/// <summary>
	/// 経験値回収判定
	/// </summary>
	/// <returns>0b100000</returns>
	constexpr uint32_t ExpGetter() { return ColMask5; }
}