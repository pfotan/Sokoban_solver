#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;

const int tile_size = 50;

pair<int,int> operator + (const pair<int,int>& x, const pair<int,int>& y){
    return make_pair(x.first + y.first, x.second + y.second);
}

sf::RenderWindow window(sf::VideoMode(700, 700), "Sokoban solver");    

struct position{
    pair<int,int> player_position;
    set<pair<int,int> > boxes_positions;

    bool are_boxes_well_placed(set<pair<int, int> >  &target_positions){
        bool result = true;
        for(pair<int, int> cur : boxes_positions){
            if(target_positions.count(cur) == 0) result = false;
        }
        return result;
    }
};

class Sokoban{
private:
    sf::Texture texture_box;
    sf::Texture texture_ball;
    sf::Texture texture_target;
    sf::Texture texture_hero;
    sf::Texture texture_ball_on_target;
    string moves;
    int moves_done;
    vector<vector<char> > board;
    set<pair<int,int> > target_positions;
    const int tile_size = 50;
    position current;
public:
    Sokoban(string filename){
        moves = "";
        moves_done = 0;
        ifstream input(filename.c_str());
        if(!input.good()){
            cout << "READING EROOR" << endl;
            exit(1);
        }
        string line;
        vector<char> current_row;
        vector<vector<char > > board;
        board.push_back(current_row);
        while(getline(input, line)){
            current_row.clear();
            current_row.push_back('#');
            for(int i=1;i<=line.size();i++){
                current_row.push_back(line[i - 1]);
            }
            board.push_back(current_row);
        }  
        parse_input(board);
        load_textures();
        moves = solve_a();
        if(moves.size() == 0){
            cout << "Puzzle is already solved!" << endl;
            exit(0);
        }
    }

    void load_textures(){
        texture_box.loadFromFile("graphics/box.png");
        texture_ball.loadFromFile("graphics/ball.jpeg");
        texture_target.loadFromFile("graphics/target.png");
        texture_hero.loadFromFile("graphics/hero_right.png");
        texture_ball_on_target.loadFromFile("graphics/ball_on_targett.jpg");
    }

    void parse_input(vector<vector<char> > &board){
        position result;
        int n = board.size() - 1;
        int m = board[1].size() - 1;
        if(n == 0){
            cout << "PARSE ERROR" << endl;
            exit(1);
        }
        for(int i=1;i<=n;i++){
            if((int)board[i].size() - 1 != m){
                cout << "PARSE ERROR" << endl;
                exit(1);
            }
            for(int j=1;j<=m;j++){
                if(board[i][j] == 'K' || board[i][j] == '+'){
                    result.player_position = make_pair(i, j);
                }
                else if(board[i][j] == 'B'){
                    result.boxes_positions.insert(make_pair(i, j));
                }
                else if(board[i][j] == 'G'){
                    target_positions.insert(make_pair(i, j));
                }
                else if(board[i][j] == '*'){
                    result.boxes_positions.insert(make_pair(i, j));
                    target_positions.insert(make_pair(i, j));
                }
                else if(board[i][j] == '+'){
                    target_positions.insert(make_pair(i, j));
                }
                else if(board[i][j] != 'W' && board[i][j] != '.'){
                    cout << "PARSE ERROR" << endl;
                    exit(1);
                }
            }
        } 
        if(max(n, m) > 14){
            cout << "Board too big!" << endl;
            exit(1);
        }
        this->board = board;
        this->current = result;
    }

    void draw_game_state(){
        sf::RectangleShape rectangle;
        int rows = board.size() - 1;
        int cols = board[1].size() - 1;
        rectangle.setSize(sf::Vector2f(cols * tile_size, rows * tile_size));
        sf::Color color(0, 0, 0);
        rectangle.setOutlineColor(color);
        rectangle.setOutlineThickness(10);
        rectangle.setOutlineColor(sf::Color::Green);
        int x_offset = (700 - cols * tile_size) / 2;
        int y_offset = (700 - rows * tile_size) / 2;
        rectangle.setPosition(x_offset, y_offset);
        window.draw(rectangle);
        for(int i=1;i<=rows;i++){
            for(int j=1;j<=cols;j++){
                sf::Sprite sprite;
                sf::Vector2u size;
                if(current.player_position == make_pair(i, j)){
                    sprite.setTexture(texture_hero);
                    size = texture_hero.getSize();
                }
                else if(current.boxes_positions.count(make_pair(i, j)) && target_positions.count(make_pair(i, j))){
                    sprite.setTexture(texture_ball_on_target);
                    size = texture_ball_on_target.getSize();
                }
                else if(current.boxes_positions.count(make_pair(i, j))){
                    sprite.setTexture(texture_ball);
                    size = texture_ball.getSize();
                }
                else if(target_positions.count(make_pair(i, j))){
                    sprite.setTexture(texture_target);
                    size = texture_target.getSize();
                }
                else if(board[i][j] == 'W'){
                    sprite.setTexture(texture_box);
                    size = texture_box.getSize();
                }
                float x_scale = 1.0 / (size.x * 1.0 / tile_size);
                float y_scale = 1.0 / (size.y * 1.0 / tile_size);
                sprite.setScale(x_scale, y_scale);
                sprite.setPosition(x_offset + (j - 1) * tile_size, y_offset + (i - 1) * tile_size);
                window.draw(sprite);
            }
        }
    }

    long long hasz(position current){
        const int mod[2] = {(int)1e9 + 7, (int)1e9 + 9};
        long long value[2] = {0, 0};
        long long p[2] = {29, 31};
        long long v[2] = {1, 1};
        value[0] = current.player_position.first * 21312381;
        value[1] = current.player_position.second * 2198347;
        for(auto cur : current.boxes_positions){
            for(int i=0;i<2;i++){
                long long cur_value = v[i];
                if(i == 0) cur_value *= cur.first;
                else cur_value *= cur.second;
                value[i] += cur_value;
                v[i] *= p[i];
                value[i] %= mod[i];
                v[i] %= mod[i];
            }
        }
        long long result = value[0] * mod[0] + value[1];
        return result;
    }

    int h(position current){
        int result = 0;
        for(auto box : current.boxes_positions){
            int min_len = 1e9;
            for(auto target : target_positions){
                min_len = min(min_len, abs(box.first - target.first) + abs(box.second - target.second));
            }
            result += min_len;
        }
        return result;
    }

    void hero_turn_right(){
        texture_hero.loadFromFile("graphics/hero_right.png");
    }

    void hero_turn_left(){
        texture_hero.loadFromFile("graphics/hero_left.png");
    }

    struct compare{
        public:
        bool operator()(const pair<int, position>&l, const pair<int,position>&r){
            return l.first > r.first;
        }
    };

    bool is_square_free_and_valid(pair<int,int> player_position, position current){
        int row = player_position.first;
        int col = player_position.second;
        int n = board.size() - 1;
        int m = board[1].size() - 1;
        if(row < 1 || row > n || col < 1 || col > m) return false;
        if(board[row][col] == 'W') return false;
        if(current.boxes_positions.count(make_pair(row, col))) return false;
        return true;
    }

    position give_next_position(position current, char direction){
        pair<int,int> player_position = current.player_position;
        map<char, pair<int, int> > directions;
        directions['U'] = {-1, 0};
        directions['D'] = {1, 0};
        directions['L'] = {0, -1};
        directions['R'] = {0, 1};
        pair<int,int> destination = directions[direction];
        player_position = player_position + destination;
        if(is_square_free_and_valid(player_position, current)){
            current.player_position = player_position;
            return current;
        }
        else{
            if(current.boxes_positions.count(player_position) == 0){
                return current;
            }   
            else{
                // we have a box in position next to us
                pair<int,int> destination_position = player_position + directions[direction];
                if(!is_square_free_and_valid(destination_position, current)){
                    return current;
                }
                else{
                    // we can finally push the box
                    current.boxes_positions.erase(player_position);
                    current.boxes_positions.insert(destination_position);
                    current.player_position = player_position;
                    return current;
                }
            }
        }
    }

    string solve_a(){
        position start = current;
        map<long long, int> g_function;
        g_function[hasz(start)] = 0;
        set<long long> visited;
        priority_queue<pair<int, position>, vector<pair<int, position> > , compare> queue;
        queue.push(make_pair(h(start), start));
        map<long long, pair<long long, char> > came_from;
        long long last = hasz(start);
        vector<char> directions = {'U', 'L', 'D', 'R'};
        int cnt = 0;
        while(queue.size()){
            cnt++;
            if(cnt > 88888){
                break;
            }
            auto current = queue.top();
            queue.pop();
            long long hash_value = hasz(current.second);
            if(visited.count(hash_value)) continue;
            if(current.second.are_boxes_well_placed(target_positions)){
                last = hash_value;
                break;
            }
            visited.insert(hash_value);
            for(char dir : directions){
                position nxt = give_next_position(current.second, dir);
                long long nxt_hash = hasz(nxt);
                if(visited.count(nxt_hash)) continue;
                int nxt_g_score = g_function[hash_value] + 1;
                auto destination = make_pair(nxt_g_score + h(nxt), nxt);
                if(g_function[nxt_hash] == 0 || nxt_g_score < g_function[nxt_hash]){
                    g_function[nxt_hash] = nxt_g_score;
                    came_from[nxt_hash] = make_pair(hash_value, dir);
                    queue.push(destination);
                }
            }
        }

        string result = "";
        while(last != hasz(start)){
            result += came_from[last].second;
            last = came_from[last].first;
        }
        reverse(result.begin(), result.end());

        return result;   
    }

    bool simulate_move(){
        char dir = moves[moves_done++];
        if(dir == 'L') hero_turn_left();
        if(dir == 'R') hero_turn_right();
        position next_state = give_next_position(current, dir);
        window.clear();
        draw_game_state();
        current = next_state;
        window.display();
        return moves_done == moves.length();
    }
};

int main(){
    window.setFramerateLimit(7);
    Sokoban sokoban_puzzle("test.txt");
    sokoban_puzzle.draw_game_state();
    while (window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
        bool finish = sokoban_puzzle.simulate_move();
        if(finish) break;
    }
    return 0;
}
