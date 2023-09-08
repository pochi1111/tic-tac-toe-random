# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include <bits/stdc++.h>
using namespace std;
//それぞれどのますに何があるか
//-1・・・なにもなし 0・・・〇 1・・・×
vector<vector<pair<int, int>>> masu(3, vector<pair<int, int>>(3, make_pair(-1, -1)));
int winner = -2;
int maru_win = 0;
int batu_win = 0;
int hikiwake = 0;
vector<int> three_step_win(2, 0);
bool win_check() {
	//行がいけるかどうか
	for (int i = 0; i < 3; i++)
	{
		if (masu[i][0].first == -1) continue;
		if (masu[i][0].first == masu[i][1].first && masu[i][1].first == masu[i][2].first) {
			winner = masu[i][0].first;

			return true;
		}
	}
	//列がいけるかどうか
	for (int i = 0; i < 3; i++)
	{
		if (masu[0][i].first == -1) continue;
		if (masu[0][i].first == masu[1][i].first && masu[1][i].first == masu[2][i].first) {
			winner = masu[0][i].first;
			return true;
		}
	}
	//ナナメチェック
	if (masu[0][0].first == masu[1][1].first && masu[1][1].first == masu[2][2].first && masu[0][0].first != -1) {
		winner = masu[0][0].first;
		return true;
	}
	if (masu[0][2].first == masu[1][1].first && masu[1][1].first == masu[2][0].first && masu[0][2].first != -1) {
		winner = masu[0][2].first;
		return true;
	}
	return false;
}
void Random_create() {
	//ランダムにくりえいと
	//先手が〇
	//〇・・・0 ×・・・1
	int now_player = 0;
	winner = -1;
	masu = vector<vector<pair<int, int>>>(3, vector<pair<int, int>>(3, make_pair(-1, -1)));
	vector<pair<int, int>> randm;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			randm.emplace_back(i, j);
		}
	}
	for (int i = 0; i < 9; i++)
	{
		//ランダムな場所を選ぶ
		int randm_number = rand() % randm.size();
		masu[randm[randm_number].first][randm[randm_number].second].first = now_player;
		masu[randm[randm_number].first][randm[randm_number].second].second = i + 1;
		//それを削除
		randm.erase(randm.begin() + randm_number);
		if (win_check()) {
			if (winner == 0) {
				//Console << U"oの{}手勝ちです"_fmt((i + 2) / 2);
				maru_win++;
				if ((i + 2) / 2 == 3) {
					three_step_win[0]++;
				}
			}
			else {
				//Console << U"xの{}手勝ちです"_fmt((i + 1) / 2);
				batu_win++;
				if ((i + 1) / 2 == 3) {
					three_step_win[1]++;
				}
			}
			return;
		}
		//プレイヤー切り替え
		now_player += 1;
		if (now_player % 2 == 0) now_player = 0;
	}
	//Console << U"引き分け";
	hikiwake += 1;
}
void Write_batu(int x, int y,int num,Font font) {
	Line{ x - 30,y - 30,x + 30,y + 30 }.draw(LineStyle::RoundCap, 12, Palette::Red);
	Line{ x + 30,y - 30,x - 30,y + 30 }.draw(LineStyle::RoundCap, 12, Palette::Red);
	font(U"{}"_fmt(num)).draw(50, x-15, y-35, Palette::Black);
}
void Write_maru(int x, int y,int num,Font font) {
	Circle{ x,y,30 }.drawFrame(10, Palette::Blue);
	font(U"{}"_fmt(num)).draw(50, x-15, y-35, Palette::Black);
}
void Write_toukei(Font font) {
	font(U"〇の勝数:{}"_fmt(maru_win)).draw(30, 450, 50, Palette::Blue);
	font(U"×の勝数:{}"_fmt(batu_win)).draw(30, 450, 100, Palette::Red);
	font(U"引き分け数:{}"_fmt(hikiwake)).draw(30, 450, 150, Palette::Black);
	font(U"ゲーム数:{}"_fmt(hikiwake+batu_win+maru_win)).draw(30, 450, 200, Palette::Black);
}
void Main()
{
	// 太文字のフォントを作成する | Create a bold font with MSDF method
	Font font = Font(FontMethod::MSDF, 48, Typeface::Bold);
	Window::SetTitle(U"〇×テスター");
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	double loop_num = 10.0;
	while (System::Update())
	{
		//マスを描画
		RoundRect{ 150, 150, 10, 300, 20 }.draw(Color{ 0,0,0 });
		RoundRect{ 250, 150, 10, 300, 20 }.draw(Color{ 0,0,0 });
		RoundRect{ 50, 250, 300, 10, 20 }.draw(Color{ 0,0,0 });
		RoundRect{ 50, 350, 300, 10, 20 }.draw(Color{ 0,0,0 });
		//図形を描画
		//200 200からスタートしていき100ずつずらしていく
		int y_tmp = 200;
		for (int i = 0; i < 3; i++)
		{
			int x_tmp = 100;
			for (int j = 0; j < 3; j++)
			{
				pair<int, int> now_masu = masu[i][j];
				if (now_masu.first == 0) {
					Write_maru(x_tmp, y_tmp,now_masu.second,font);
				}
				else if (now_masu.first == 1) {
					Write_batu(x_tmp, y_tmp,now_masu.second,font);
				}
				x_tmp += 100;
			}
			y_tmp += 100;
		}
		//下の文字
		if (winner == -2) {
			//初期状態なのでとくになし
			font(U"右クリックで新しい場面").draw(40, 200, 500, Palette::Black);
		}
		else if (winner == 0) {
			//丸の価値
			font(U"〇の勝ち！").draw(50, 500, 500, Palette::Blue);
		}
		else if (winner == 1) {
			//バツのかち
			font(U"×の勝ち！").draw(50, 500, 500, Palette::Red);
		}
		else {
			//あいこ
			font(U"引き分け！").draw(50, 500, 500, Palette::Black);
		}
		if (MouseR.down()) {
			Random_create();
		}
		if (MouseR.down() && MouseL.down()) {
			Console << U"oの3手勝ち:{}"_fmt(three_step_win[0]);
			Console << U"xの3手勝ち:{}"_fmt(three_step_win[1]);
		}
		//スライダー
		SimpleGUI::Slider(U"{:.2f}"_fmt(round(loop_num)), loop_num, 5.0, 100000.0, Vec2{ 500, 300 }, 60, 150);
		
		//繰り返すボタン
		if (SimpleGUI::Button(U"繰り返す", Vec2{ 500, 350 }))
		{
			for (int i = 0; i < round(loop_num); i++)
			{
				Random_create();
			}
		}
		Write_toukei(font);
		// "Licenses" ボタンが押されたら
		if (SimpleGUI::Button(U"Licenses", Vec2{ 20, 20 }))
		{
			// ライセンス情報を表示
			LicenseManager::ShowInBrowser();
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
