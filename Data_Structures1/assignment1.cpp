#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

typedef struct Cell{
    char shape;
    int center_row, center_col;
    int shape_size;
} Cell;

Cell create_cell(char shape, int center_row, int center_col, int shape_size){
    Cell cell;
    cell.shape = shape;
    cell.center_row = center_row;
    cell.center_col = center_col;
    cell.shape_size = shape_size;
    return cell;
}

Cell copy_cell(Cell cell){
    Cell new_cell;
    new_cell.shape = cell.shape;
    new_cell.center_row = cell.center_row;
    new_cell.center_col = cell.center_col;
    new_cell.shape_size = cell.shape_size;
    return new_cell;
}

typedef struct Map{
    int num_rows;
    int num_cols;
    Cell **map;
    FILE *grid_file;

    void load_grid(char *path){
        grid_file = fopen(path, "r");
        fscanf(grid_file, "%d %d\n", &num_rows, &num_cols);

        map = new Cell*[num_rows];
        for(int i = 0; i < num_rows; ++i){
            map[i] = new Cell[num_cols];
            for(int j = 0; j < num_cols; ++j){
                Cell cell; cell.shape = '*';
                cell.center_row = -1; cell.center_col = -1;
                cell.shape_size = -1;
                map[i][j] = cell;
            }
        }

        cout << "A grid is created: "<< num_rows << " " << num_cols << endl;

    }

    bool border_error_check(int r, int c){
        if( r < 0 || r >= num_rows || c < 0 || c >= num_cols ) return true;
        return false;
    }

    bool conflict_error_check(int r, int c){
        if( map[r][c].shape != '*' ) return true;
        return false;
    }

    bool error_check(Cell cell){
        int error = 0;
        if( cell.shape == '+' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                if( border_error_check( cell.center_row + i, cell.center_col ) ){
                    error = 1;
                    break;
                }
                else if( conflict_error_check( cell.center_row + i, cell.center_col ) ){
                    error = 2;
                    break;
                }
                if( border_error_check( cell.center_row, cell.center_col + i ) ){
                    error = 1;
                    break;
                }
                else if( conflict_error_check( cell.center_row, cell.center_col + i ) ){
                    error = 2;
                    break;
                }
            }
        }
        else if( cell.shape == 'x' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                if( border_error_check( cell.center_row - i, cell.center_col + i ) ){
                    error = 1;
                    break;
                }
                else if( conflict_error_check( cell.center_row - i, cell.center_col + i ) ){
                    error = 2;
                    break;
                }
                if( border_error_check( cell.center_row + i, cell.center_col + i ) ){
                    error = 1;
                    break;
                }
                else if( conflict_error_check( cell.center_row + i, cell.center_col + i ) ){
                    error = 2;
                    break;
                }
            }
        }
        else if( cell.shape == '-' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                if( border_error_check( cell.center_row, cell.center_col + i ) ){
                    error = 1;
                    break;
                }
                else if( conflict_error_check( cell.center_row, cell.center_col + i ) ){
                    error = 2;
                    break;
                }
            }
        }
        else if( cell.shape == '/' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                if( border_error_check( cell.center_row - i, cell.center_col + i ) ){
                    error = 1;
                    break;
                }
                else if( conflict_error_check( cell.center_row - i, cell.center_col + i ) ){
                    error = 2;
                    break;
                }
            }
        }
        if(error == 1){
            cout << "BORDER ERROR: ";
            return true;
        }
        else if(error == 2){
            cout << "CONFLICT ERROR: ";
            return true;
        }
        else{
            cout << "SUCCESS: ";
            return false;
        }
    }

    void draw_shape(Cell cell, int mode){
        Cell empty = create_cell('*', -1, -1, -1);
        if( cell.shape == '+' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                map[cell.center_row + i][cell.center_col] = mode == 1 ? copy_cell(cell) : empty;
                map[cell.center_row][cell.center_col + i] = mode == 1 ? copy_cell(cell) : empty;
            }
        }
        else if( cell.shape == 'x' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                map[cell.center_row - i][cell.center_col + i] = mode == 1 ? copy_cell(cell) : empty;
                map[cell.center_row + i][cell.center_col + i] = mode == 1 ? copy_cell(cell) : empty;
            }
        }
        else if( cell.shape == '-' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                map[cell.center_row][cell.center_col + i] = mode == 1 ? copy_cell(cell) : empty;
            } 
        }
        else if( cell.shape == '/' ){
            for(int i = -cell.shape_size; i <= cell.shape_size; ++i){
                map[cell.center_row - i][cell.center_col + i] = mode == 1 ? copy_cell(cell) : empty;
            }
        }
    }

    void add_shapes(){

        while( !feof( grid_file ) ){

            Cell cell;
            fscanf(grid_file, "%c %d %d %d\n", &cell.shape, &cell.center_row,
                                               &cell.center_col, &cell.shape_size);
            cell.center_row -= 1;
            cell.center_col -= 1;

            if( error_check(cell) ){
                cout << "Operator " << cell.shape << " with size " << cell.shape_size
                << " can not be placed on (" << cell.center_row + 1 << "," << cell.center_col + 1
                << ")." << endl;
            }
            else{
                draw_shape(cell, 1);
                cout << "Operator " << cell.shape << " with size " << cell.shape_size
                << " is placed on (" << cell.center_row + 1 << "," << cell.center_col + 1 << ")." << endl;
            }

        }

        fclose(grid_file);

    }

    void move_shapes(char *path){

        FILE *instructions = fopen(path, "r");

        while( !feof( instructions ) ){

            int row, col, move_by;
            char *ins = new char[4];

            fscanf(grid_file, "%s %d %d %d\n", ins, &row, &col, &move_by);

            row -= 1;
            col -= 1;

            int r, c;
            r = 0;
            c = 0;
            r = ins[2] == 'U' ? -move_by : r;
            r = ins[2] == 'D' ? move_by : r;
            c = ins[2] == 'R' ? move_by : c;
            c = ins[2] == 'L' ? -move_by : c;

            Cell cell = map[row][col];

            draw_shape(cell, 0);

            Cell moved_shape = create_cell(cell.shape, cell.center_row + r, cell.center_col + c, cell.shape_size);

            if( error_check(moved_shape) ){
                draw_shape(cell, 1);
                cout << cell.shape << " can not be moved from (" << cell.center_row + 1 << "," << cell.center_col + 1
                << ") to (" << moved_shape.center_row + 1 << "," << moved_shape.center_col + 1 << ")." << endl;
            }
            else{
                draw_shape(moved_shape, 1);
                cout << cell.shape << " moved from (" << cell.center_row + 1 << "," << cell.center_col + 1
                << ") to (" << moved_shape.center_row + 1 << "," << moved_shape.center_col + 1 << ")." << endl;
            }   

            delete[] ins;

        }

    }

    void print(){
        for(int i = 0; i < num_rows; ++i){
            for(int j = 0; j < num_cols; ++j){
                cout << map[i][j].shape << " ";
            }
            cout << endl;
        }
    }

} Map;

int main(int argc, char **argv){

    Map map;
    map.load_grid(argv[1]);
    map.add_shapes();
    map.move_shapes(argv[2]);
    //map.print();

    return 0;
}