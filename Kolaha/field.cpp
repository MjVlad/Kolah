#include "field.h"
using std::cout;
using std::cin;
using std::ostream;
using std::endl;

field::field() {
	std::fill_n(fl, pl_cell, csooc);
	/*for (int i = 0; i < pl_cell; i++)
		fl[i].set(csooc);*/
	std::fill_n(fl + 7, pl_cell, csooc);
	/*for (int i = pl_cell + 1; i < fl_size - 1; i++)
		fl[i].set(csooc);*/
}

field::field(const field& fld) {
	//std::copy(fl, fl + fl_size - 1, fld.fl);
	for (int i = 0; i < fl_size; i++)
		fl[i].set(fld.fl[i].get());
}

field& field::operator=(field& r){
	for (int i = 0; i < fl_size; i++)
		fl[i].set(r.fl[i].get());
	return *this;
}

ostream& operator<<(ostream& out, field& game) {
	out << endl << "		";
	for (int i = fl_size - 2; i > pl_cell; i--)
		out << game.fl[i].get() << "	";
	out << endl;
	out << "	" << game.fl[fl_size - 1].get();
	for (int i = 0; i <= pl_cell; i++, out << "	");
	out << game.fl[pl_cell].get() << endl << "		";
	for (int i = 0; i < pl_cell; i++)
		out << game.fl[i].get() << "	";
	out << endl;
	return out;
}

short game::if_win(player& pl) {
	int win_number = pl_cell * csooc;
	if (pl.own_kol->get() > win_number) {
		return 1;//"Player number: " << (pl.pl_id + 1) << " winner!!!!!!"
	}
	cell* cur;
	short counter = 0;
	cur = pl.own_first;
	for (cur; cur != pl.own_kol; ++cur) {
		counter += cur->get();
	}
	if (counter != 0) {
		cur = pl.enemy_kol - 6;
		for (cur; cur != pl.enemy_kol; ++cur) {
			if (cur->get() > 0)
				return 0;
		}
	}
	cur = pl.enemy_kol - 6;
	int tmp = 0;
	for (cur; cur != pl.enemy_kol; ++cur)
		tmp += cur->pick_up();
	pl.enemy_kol->operator+=(tmp);
	tmp = 0;
	cur = pl.own_first;
	for (cur; cur != pl.own_kol; ++cur)
		tmp += cur->pick_up();
	pl.own_kol->operator+=(tmp);
	if (pl.own_kol->get() > win_number) {
		return 1;//"Player number: " << (pl.pl_id + 1) << " winner!!!!!!"
	}
	else if (pl.enemy_kol->get() > win_number) {
		return 3; //"Player number: " << (((pl.pl_id + 1) % 2) + 1) << " winner!!!!!!"
	}
	else {
		return 2; // "Draw in the game"
	}
}


short game::turn(field& fl, player& pl) {
	cout << fl;
	short id_cell;
	switch (pl.AI_lvl) {
	case 1:
		id_cell = pl.move_selection_AI_1();
		break;
	case 2:
		id_cell = pl.move_selection_AI_2();
		break;
	case 3:
		id_cell = pl.move_selection_AI_3(fl);
		break;
	case 4:
		id_cell = pl.move_selection_AI_4(fl);
		break;
	case 5:
		id_cell = pl.move_selection_AI_5(fl);
		break;
	default:
		id_cell = pl.move_selection();
	}
	cell* cur = pl.own_first + id_cell;
	if (!cur->get()) {
		cout << "The selected cell is empty";
		return 1;
	}
	int count_stn = cur->pick_up();
	for (count_stn; count_stn > 0;) {
		if (pl.pl_id) {
			if (cur < pl.own_kol) cur++;
			else cur = pl.enemy_kol - pl_cell;
		}
		else {
			if (cur < pl.enemy_kol) cur++;
			else cur = pl.own_first;
		}
		if (cur != pl.enemy_kol) {
			cur->operator++();
			count_stn--;
		}
	}
	if (cur == pl.own_kol) {
		if (this->if_win(pl)) return 0;
		cout << "Player " << (pl.pl_id + 1) << " replay" << endl;
		//turn(fl, pl);
		return 1;
	}
	if (cur->get() == 1) {
		if (this->if_win(pl)) return 0;
		__int64 tmp = (cur - pl.own_first) + 1;
		if (tmp <= 0 || tmp > pl_cell) return 0;
		cell* cur_mirror = pl.enemy_kol - tmp;
		if (!cur_mirror->get())
			return 0;
		int temp;
		temp = cur->pick_up();
		temp += cur_mirror->pick_up();
		pl.own_kol->operator+=(temp);
		if (this->if_win(pl)) return 0;
	}
	return 0;
}

short game::start(short dif_id_type1, short dif_id_type2) {
	field fl;
	player pl[2];
	pl[0].set(0, dif_id_type1, fl);
	pl[1].set(1, dif_id_type2, fl);
	for (int i = 0;; i = (i + 1) % 2) {
		while (turn(fl, pl[i]));
		short vic_cond = if_win(pl[i]);
		if (!vic_cond) {
			int tmp = if_win(pl[(i + 1) % 2]);
			if (!tmp)
				continue;
		}
		cout << fl;
		switch (vic_cond) {
		case 1:
			cout << "Player number: " << (pl[i].pl_id + 1) << " winner!!!!!!" << endl;
			return pl[i].pl_id;
		case 3:
			cout << "Player number: " << (pl[(i + 1) % 2].pl_id + 1) << " winner!!!!!!" << endl;
			return pl[(i + 1) % 2].pl_id;
		default:
			cout << "Draw in the game" << endl;
			return -1;
		}
	}
}

player::player() {
	pl_id = AI_lvl = -1;
	own_first = own_kol = enemy_kol = nullptr;
}

void player::set(short id, short AI, field& fl) {
	pl_id = id;
	AI_lvl = AI;
	if (pl_id == 0) {
		own_kol = &fl.fl[pl_cell];
		enemy_kol = &fl.fl[fl_size - 1];
	}
	else {
		own_kol = &fl.fl[fl_size - 1];
		enemy_kol = &fl.fl[pl_cell];
	}
	own_first = own_kol - pl_cell;
}

short player::move_selection() {
	short tmp;
	bool indx = 0;
	do {
		if (indx) cout << "Impossible move. Please try again" << endl;
		else indx++;
		cout << "Player number " << (pl_id + 1) << " make a move: ";
		cin >> tmp;
		cout << endl;
	} while (tmp <= 0 || tmp > 6);
	return --tmp;
}

short player::move_selection_AI_1() {
	cout << "Player(AI) number " << (this->pl_id + 1) << " make a move: ";
	srand(clock());
	short s_move;
	cell* temp;
	do {
		s_move = (rand() % 6);
		temp = this->own_first + s_move;
	} while (!temp->get());
	cout << s_move + 1 << endl << endl;
	return s_move;
}

short player::if_bigsteal() {
	short s_move = -1;
	cell* cur;
	for (int i = 0; i < pl_cell; i++) {
		cur = own_first + i;
		int st = cur->get();
		if (!st) continue;
		int cur_id = (st + i) % (fl_size - 1);
		if (cur_id < pl_cell) {
			cell* tmp = own_first + cur_id;
			if (tmp->get()) continue;
			tmp = enemy_kol - cur_id - 1;
			if (tmp->get()) s_move = i;
		}
	}
	if (s_move >= 0) {
		cout << s_move + 1 << endl << endl;
		return s_move;
	}
	return -1;
}

short player::if_doubleturn() {
	short s_move = -1;
	cell* cur;
	for (int i = 0; i < pl_cell; i++) {
		cur = own_first + i;
		int st = cur->get();
		if (!st) continue;
		int cur_id = (st + i) % (fl_size - 1);
		if (cur_id == pl_cell) s_move = i;
	}
	if (s_move >= 0) {
		cout << s_move + 1 << endl << endl;
		return s_move;
	}
	return -1;
}

short player::rand_from_max() {
	short s_move;
	cell* cur;
	short local_max = 0;
	short count_local_max = 0;
	for (int i = 0; i < pl_cell; i++) {
		cur = this->own_first + i;
		if (cur->get() > local_max) {
			local_max = cur->get();
			s_move = i;
			count_local_max = 0;
		}
		if (cur->get() == local_max)
			count_local_max++;
	}
	if (count_local_max > 1) {
		srand(clock());
		short tmp = (rand() % count_local_max) + 1;
		for (s_move = 0; tmp; s_move++) {
			cur = this->own_first + s_move;
			if (cur->get() == local_max)
				tmp--;
		}
		s_move--;
	}
	cout << s_move + 1 << endl << endl;
	return s_move;
}

short player::move_selection_AI_2() {
	cout << "Player(AI) number " << (this->pl_id + 1) << " make a move: ";
	short s_move = if_bigsteal();
	if (s_move != -1) return s_move;
	s_move = if_doubleturn();
	if (s_move != -1) return s_move;
	s_move = rand_from_max();
	return s_move;
}

short field::turn(short move, short pl_id) {
	short id_own_col = 6;
	short id_en_col = 13;
	if (pl_id == 1) {
		move += pl_cell + 1;
		id_own_col = 13;
		id_en_col = 6;
	}
	if (fl[move].get() == 0) return 0; //impossible move
	short count_stn = fl[move].pick_up();
	for (count_stn; count_stn > 0;) { //turn
		move = (move + 1) % fl_size;
		if (move != id_en_col) {
			++fl[move];
			count_stn--;
		}
	}
	if (move == id_own_col) return 2; //double turn
	if (fl[move].get() == 1) {
		if (move < id_own_col && move >= (id_en_col + 1) % 14) {//need to test
			short tmp = fl_size - move - 2;
			if (fl[tmp].get() > 0) {
				fl[id_own_col] += fl[tmp].pick_up() + fl[move].pick_up();
			}
		}
	}
	return 1; //complete turn
}

short field::col_stones(short& pl_id) const {
	if (pl_id) return fl[13].get();
	return fl[6].get();
}

std::pair<float, short> player::recurs_procces(const field& fl, short pl_id, short deep, short preset) {
	std::pair<float, short> res;
	res.second = -1;
	if (!deep) {
		res.first = fl.col_stones(this->pl_id);
		return res;
	}
	std::multimap<float, short> options;
	for (int i = 0; i < pl_cell; i++) {
		field fld(fl);
		short id = fld.turn(i, pl_id);
		if (id == 0) continue;
		std::pair<float, short> tmp;
		if (id == 2)
			tmp = recurs_procces(fld, pl_id, deep, preset);
		else {
			tmp = recurs_procces(fld, ((pl_id + 1) % 2), (deep - 1), preset);
		}
		tmp.second = i;
		options.insert({ tmp });
	}
	if (deep % 2 == 0) {
		float mat = 0;
		for (auto it : options) {
			mat += it.first;
		}
		mat /= options.size();
		res.first = mat;
	}
	else {
		auto it = options.end();
		--it;
		res = *it;
	}
	return res;
}

short player::move_selection_AI_3(const field& fl) {
	cout << "Player(AI) number " << (this->pl_id + 1) << " make a move: ";
	std::pair<float, short> res = recurs_procces(fl, this->pl_id, (2 * R_DEEP + 1));
	cout << res.second + 1 << endl << endl;
	return res.second;
}

short player::move_selection_AI_4(const field& fl){
	cout << "Player(AI) number " << (this->pl_id + 1) << " make a move: ";
	std::pair<float, short> res = recurs_procces(fl, this->pl_id, (2 * 2 + 1));
	cout << res.second + 1 << endl << endl;
	return res.second;
}

short player::move_selection_AI_5(const field& fl){
	cout << "Player(AI) number " << (this->pl_id + 1) << " make a move: ";
	std::pair<float, short> res = recurs_procces(fl, this->pl_id, (2 * 3 + 1));
	cout << res.second + 1 << endl << endl;
	return res.second;
}
