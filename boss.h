#include <vector>
#include <string>
#include <climits>
#include <memory>  // 添加智能指针支持
using namespace std;
struct Skill {
    string name;
    int damage;
    int cooldown;
    int cost;
    Skill(string n, int d, int cd, int c = 0) : name(n), damage(d), cooldown(cd), cost(c) {}
};

struct GameState {
    vector<int> boss_hp;
    int resources;
    int turns_used;
    vector<int> cooldowns;
    vector<string> sequence;
    shared_ptr<const vector<Skill>> skills;  // 使用智能指针共享技能列表
    explicit GameState(shared_ptr<const vector<Skill>> s)
            : resources(0), turns_used(0), skills(s) {}

    bool operator>(const GameState& other) const {
        return (turns_used + estimate_remaining_turns()) >
               (other.turns_used + other.estimate_remaining_turns());
    }
    int estimate_remaining_turns() const;
    bool all_bosses_defeated() const;
    bool is_valid() const;
};
struct GameConfig {
    shared_ptr<vector<Skill>> skills;  // 使用智能指针
    vector<int> boss_hp;
    int initial_resources;
};

bool load_game_data(const string& filename, GameConfig& config);
vector<string> find_optimal_strategy(const GameConfig& config, int max_turns);
int boss_fighting();