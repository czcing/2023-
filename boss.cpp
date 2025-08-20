#include "boss.h"
#include <queue>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <cmath>
#include <iostream>
#include <json.hpp>
using namespace std;
using json = nlohmann::json;
int GameState::estimate_remaining_turns() const {
    if (boss_hp.empty()) return 0;
    int total_hp = accumulate(boss_hp.begin(), boss_hp.end(), 0,[](int sum, int hp) { return sum + (hp > 0 ? hp : 0); });
    if (total_hp <= 0) return 0;
    int total_damage = 0;
    for (const auto& skill : *skills) {
        total_damage += skill.damage;
    }
    double average_damage = static_cast<double>(total_damage) / skills->size();
    if (average_damage <= 0) {
        return static_cast<int>(ceil(total_hp / 5.0));
    }
    return static_cast<int>(ceil(total_hp / average_damage));
}

bool GameState::all_bosses_defeated() const {
    return all_of(boss_hp.begin(), boss_hp.end(), [](int hp) { return hp <= 0; });
}

bool GameState::is_valid() const {
    return resources >= 0;
}

bool load_game_data(const string& filename, GameConfig& config) {
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return false;
        }

        json data;
        file >> data;

        config.boss_hp = data["B"].get<vector<int>>();

        config.skills = make_shared<vector<Skill>>();
        const auto& player_skills = data["PlayerSkills"];

        for (size_t i = 0; i < player_skills.size(); ++i) {
            const auto& skill_data = player_skills[i];
            if (skill_data.size() >= 2) {
                int damage = skill_data[0];
                int cooldown = skill_data[1];
                int cost;
                if (skill_data.size() > 2) {
                    cost = skill_data[2];
                } else {
                    cost = 0;
                }
                config.skills->emplace_back(to_string(i), damage, cooldown, cost);
            }
        }

        config.initial_resources = 10;
        return true;
    } catch (const exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        return false;
    }
}

vector<string> find_optimal_strategy(const GameConfig& config, int max_turns) {
    priority_queue<GameState, vector<GameState>, greater<GameState>> pq;
    GameState initial(config.skills);
    initial.boss_hp = config.boss_hp;
    initial.resources = config.initial_resources;
    initial.turns_used = 0;
    initial.cooldowns.resize(config.skills->size(), 0);
    initial.sequence.clear();
    pq.push(initial);
    int min_turns = 10000;
    vector<string> best_sequence;
    while (!pq.empty() && best_sequence.empty()) {
        GameState current = pq.top();
        pq.pop();
        if (current.turns_used + current.estimate_remaining_turns() >= min_turns) {
            continue;
        }
        if (current.all_bosses_defeated()) {
            best_sequence = current.sequence;
            continue;
        }
        if (current.turns_used >= max_turns) {
            continue;
        }
        for (size_t i = 0; i < current.skills->size(); ++i) {
            const Skill &skill = (*current.skills)[i];
            if (current.cooldowns[i] == 0 && current.resources >= skill.cost) {
                GameState next = current;
                for (int &hp: next.boss_hp) {
                    if (hp > 0) {
                        hp = max(0, hp - skill.damage);
                        break;
                    }
                }
                next.resources -= skill.cost;
                next.turns_used++;
                next.sequence.push_back(skill.name);
                for (size_t j = 0; j < next.cooldowns.size(); ++j) {
                    if (next.cooldowns[j] > 0) {
                        next.cooldowns[j]--;
                    }
                }
                next.cooldowns[i] = skill.cooldown;
                if (next.is_valid()) {
                    pq.push(move(next));  // Using move semantics for efficiency
                }
            }
        }
    }
    return best_sequence;
}
int boss_fighting() {
    cout<<endl;
    // 加载游戏配置
    GameConfig config;
    if (!load_game_data("D:\\code\\GAME\\maze.json", config)) {
        cerr << "Failed to load game data, please check JSON file" << endl;
        return 0;
    }
    // 设置最大回合数
    int max_turns = 100;
    // 寻找最优策略
    vector<string> optimal_sequence = find_optimal_strategy(config, max_turns);
    if (optimal_sequence.empty()) {
        cout << "Cannot defeat all BOSSes within " << max_turns << " turns." << endl;
        return -1;
    }
    // 初始化游戏状态
    GameState simulator(config.skills);
    simulator.boss_hp = config.boss_hp;
    simulator.resources = config.initial_resources;
    simulator.turns_used = 0;
    simulator.cooldowns.resize(config.skills->size(), 0);
    // 输出初始BOSS血量
    cout << "Initial BOSS HP: ";
    for (size_t i = 0; i < simulator.boss_hp.size(); ++i) {
        cout << "BOSS" << i+1 << ":" << simulator.boss_hp[i] << " ";
    }
    cout << endl;
    // 记录哪些BOSS已被击败
    vector<bool> boss_dead(simulator.boss_hp.size(), false);
    // 模拟战斗过程
    for (const string& skill_name : optimal_sequence) {
        // 寻找当前使用的技能
        size_t skill_idx = 0;
        for (; skill_idx < config.skills->size(); ++skill_idx) {
            if ((*config.skills)[skill_idx].name == skill_name) break;
        }
        const Skill& skill = (*config.skills)[skill_idx];
        // 应用技能效果
        for (size_t i = 0; i < simulator.boss_hp.size(); ++i) {
            if (simulator.boss_hp[i] > 0) {
                int old_hp = simulator.boss_hp[i];
                simulator.boss_hp[i] = max(0, simulator.boss_hp[i] - skill.damage);
                // 检查是否有BOSS被击败
                if (old_hp > 0 && simulator.boss_hp[i] <= 0 && !boss_dead[i]) {
                    cout << "BOSS" << i+1 << " was defeated by Player!" << endl;
                    boss_dead[i] = true;
                }
                break;
            }
        }

        // 更新冷却时间和资源
        simulator.resources -= skill.cost;
        simulator.turns_used++;
        for (size_t j = 0; j < simulator.cooldowns.size(); ++j) {
            if (simulator.cooldowns[j] > 0) {
                simulator.cooldowns[j]--;
            }
        }
        simulator.cooldowns[skill_idx] = skill.cooldown;
        cout <<"Turn"<<simulator.turns_used << ": Used skill "<< skill_name<<endl;
        cout << "HP: ";
        for (size_t i = 0; i < simulator.boss_hp.size(); ++i) {
                cout <<"Boss"<<i+1<<":"<<simulator.boss_hp[i]<<" ";
            }
        cout << endl;
    }
    // 输出成功击败BOSS的信息
    cout << "Successs Defeat the boss"  <<endl;
    cout<<"The fighting used"<<optimal_sequence.size() <<" turns"<<endl;
    return optimal_sequence.size();
}