#pragma once
#include <iostream>
#include <ctime>
#include <map>
#include<algorithm>

#define pl_cell 6
#define fl_size (pl_cell * 2 + 2)
#define csooc 6 //count_stones_on_one_cell
#define R_DEEP 1

class cell{
	//friend class player;//for tests
private:
	int numb_stones;
public:
	cell(){ numb_stones = 0; }
	cell(int nb_stones) { numb_stones = nb_stones; }
	void set(int nb_stones){ numb_stones = nb_stones; }

	int get() const { return numb_stones; }
	int pick_up() {
		int tmp = numb_stones;
		numb_stones = 0;
		return tmp;
	}

	void operator++() { numb_stones++; }
	void operator+=(int r) { numb_stones += r; }
};

class field {
	friend class player;
	friend class game;
private:
	cell fl[fl_size];
public:
	field();
	field(const field& fld);

	short turn(short move, short pl_id);
	short col_stones(short& pl_id) const;

	field& operator=(field& r);

	friend std::ostream& operator<<(std::ostream& out, field& game);
};


class game {
private:
	short if_win(player& pl);
	short turn(field& fl, player& pl);
public:
	short start(short dif_id_type1, short dif_id_type2);
};

class player {
protected:
	short AI_lvl;
	short pl_id;
	cell* own_kol;
	cell* own_first;
	cell* enemy_kol;
	short if_bigsteal();
	short if_doubleturn();
	short rand_from_max();
	std::pair<float, short> recurs_procces(const field& fl, short pl_id, short deep, short preset = 0);
public:
	friend class game;
	player();
	void set(short id, short AI, field& fl);
	short move_selection();
	short move_selection_AI_1();
	short move_selection_AI_2();
	short move_selection_AI_3(const field& fl);
	short move_selection_AI_4(const field& fl);
	short move_selection_AI_5(const field& fl);
};