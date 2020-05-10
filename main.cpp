#include <iostream>
#include <cmath>
#include <vector>
using uint = unsigned int;
using dvec = std::vector<double>;
const int INF = INT_MAX;


class TSP_cost_matrix {
public:
    std::vector<std::vector<double>> cost_matrix;
    uint low_bound = 0;
    std::vector<std::pair<int, int>> next_path_solution;
    std::vector<int> sorted;


    void reduce_row(uint row_num) {
        int min_row = cost_matrix[row_num][0];
        int c = 0;
        for (int i = 0; i < cost_matrix.size(); i++) {
            if (cost_matrix[row_num][i] < min_row && cost_matrix[row_num][i]!= INF)
                min_row = cost_matrix[row_num][i];
            else { c +=1;}
        }

        if (c==cost_matrix.size()){ min_row = 0;}

        low_bound += min_row;
        for (int j = 0; j < cost_matrix.size(); j++) {
            if (cost_matrix[row_num][j] != INF)
                cost_matrix[row_num][j] -= min_row;
        }
    };

    void reduce_all_rows() {
        for (int i = 0; i < cost_matrix.size(); i++) {
            reduce_row(i);
        }
    };

    void reduce_col(uint col_num) {
        int min_col = cost_matrix[0][col_num];
        int c = 0;
        for (int i = 0; i < cost_matrix.size(); i++) {
            if (cost_matrix[i][col_num] < min_col && cost_matrix[i][col_num]!= INF)
                min_col = cost_matrix[i][col_num];
            else { c +=1;}
        }
        if (c==cost_matrix.size()){ min_col = 0;}
        low_bound += min_col;
        for (int j = 0; j < cost_matrix.size(); j++){
            if (cost_matrix[j][col_num] != INF)
                cost_matrix[j][col_num] -= min_col;
        }
    };

    void reduce_all_cols(){
        for(int i=0; i<cost_matrix.size(); i++){
            reduce_col(i);
        }
    };


    int find_min_in_row(int row_num, int col_num){
        int min_val = INF;
        for (int i=0; i< cost_matrix.size(); i++){
            if (cost_matrix[row_num][i] <= min_val && i!= col_num){
                min_val = cost_matrix[row_num][i] ;
            }
        }
        return min_val;
    };

    int find_min_in_col(int row_num, int col_num){
        int min_val = INF;
        for (int i=0; i< cost_matrix.size(); i++){
            if (cost_matrix[i][col_num] <= min_val && i!= row_num){
                min_val = cost_matrix[i][col_num] ;
            }
        }
        return min_val;
    };

    int sum_of_mins(int row_num, int col_num){
        return find_min_in_row(row_num, col_num) + find_min_in_col(row_num, col_num);
    };


    void forbid_path(int chosen_row, int chosen_col){
        for (int i=0; i<cost_matrix.size(); i++){
            for(int j=0; j<cost_matrix.size(); j++){
                if( i == chosen_row || j == chosen_col){
                    cost_matrix[i][j] = INF;
                }
            }
        }
        cost_matrix[chosen_col][chosen_row]=INF;
    };

    void find_next_path(){
        int maksimum = -1;
        int output_i, output_j;
        for(int i=0; i<cost_matrix.size(); i++){
            for(int j=0; j<cost_matrix.size(); j++){
                if(cost_matrix[i][j]==0 && sum_of_mins(i,j)>maksimum){
                    maksimum = sum_of_mins(i,j);
                    output_i = i;
                    output_j = j;
                }
            }
        }
        next_path_solution.push_back({output_i+1,output_j+1});
        forbid_path(output_i,output_j);
    };



    void sort_path(){
        int c = next_path_solution[0].second;
        sorted.push_back(next_path_solution[0].first);
        for(int i = 0; i < next_path_solution.size(); i++) {
            for (int j = 0; j < next_path_solution.size(); j++) {
                if (c == next_path_solution[j].first){
                    if (c == next_path_solution[0].first ) break;
                    sorted.push_back(next_path_solution[j].first);
                    c= next_path_solution[j].second;
                }
            }
        }
        sorted.push_back(sorted[0]);
    };

    std::vector<int> find_best_path(){
        for(int i=0; i<cost_matrix.size()-1; i++){
            reduce_all_rows();
            reduce_all_cols();
            find_next_path();

        }

        for(int i = 0; i < cost_matrix.size(); i++) {
            for (int j = 0; j < cost_matrix.size(); j++) {
                if(cost_matrix[i][j]!= INF){
                    low_bound+=cost_matrix[i][j];
                    next_path_solution.push_back({i+1,j+1});

                }

            }
        }
        sort_path();

        return sorted;
    };



};

std::vector<int> tsp(std::vector<std::vector<double>> cost_matrix){
    TSP_cost_matrix cost_matrix_object;
    cost_matrix_object.cost_matrix = cost_matrix;
    for (int i = 0; i < cost_matrix_object.sorted.size(); i++) {
        std::cout << cost_matrix_object.sorted[i] << " ";
    }
    return cost_matrix_object.find_best_path();
}


int main(){

    std::vector<std::vector<double>> test_matrix{
            {INF, 10,  8,   19,  12},
            {10,  INF, 20,  6,   3},
            {8,   20,  INF, 4,   2},
            {19,  6,   4,   INF, 7},
            {12,  3,   2,   7,   INF}
    };

    std::vector<int> best_path = tsp(test_matrix);
    std::cout << "best path is:" << std::endl;
    for (int i = 0; i < best_path.size(); i++) {
        std::cout << best_path[i] << " ";
    }

    return 0;


}