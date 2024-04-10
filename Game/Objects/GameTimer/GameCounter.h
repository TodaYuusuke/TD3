#pragma once

#include <Adapter.h>

/// <summary>
/// 0 ~ 9 までの数字を画面に映す
/// </summary>
class GameCounter
{
public: //*** パブリック関数 ***//

	// わかりやすく wtf とフラグを設定
	lwp::WorldTransform transform_;

	bool isActive_ = false;

	/// <summary>
	/// 初期化
	/// <para>表示する数字の画像ファイルを指定する</para>
	/// </summary>
	/// <param name="textureName">resource/texture/count/...以降の種類名</param>
	void Initialize(const std::string& textureName = "Normal");

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カウントをリセットする
	/// </summary>
	/// <param name="cnt">リセットする値</param>
	void Reset(int cnt = 0);

	/// <summary>
	/// 増分
	/// <para>桁上がりすると true</para>
	/// </summary>
	/// <returns>桁上がりすると true</returns>
	bool Increase();
	/// <summary>
	/// 減分
	/// <para>桁下がりすると true</para>
	/// </summary>
	/// <returns>桁下がりすると true</returns>
	bool Decrease();

	/// <summary>
	/// 今のカウント数を取得
	/// </summary>
	/// <returns></returns>
	int GetCount() const { return count_; }

	/// <summary>
	/// テクスチャを変更するときに使う
	/// <para>途中で変える事とか無いと思う</para>
	/// </summary>
	/// <param name="textureName">変えたいフォントの名前</param>
	void ChangeTexture(const std::string& textureName);

private: //*** プライベート変数 ***//

	// 今のカウント
	int count_ = 0;
	// スプライト
	LWP::Primitive::Sprite sprite_;

	// フォントとかを指定出来たらいいな
	std::string textureName_;

	// 変わる前のカウント
	int preCount_ = 0;

private: //*** プライベート関数 ***//

	/// <summary>
	/// テクスチャ名と数字を合成してファイルパスを返す関数
	/// <para>(resource/texture/)count/ + テクスチャ名 + 数字 + .png</para>
	/// </summary>
	std::string GetTextruePass();

	/// <summary>
	/// カウントが変わった時に呼び出す
	/// <para>今の数字を変える</para>
	/// </summary>
	void ChangeTexture();

};

