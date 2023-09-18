import random

def galton_board(rows, cols):
    last_row = [[] for _ in range(cols)]

    
    def drop_ball(start_col):
        grid = [[' ' for _ in range(cols)] for _ in range(rows)]
        
        row = 0
        col = start_col

        while row < rows:
            grid[row][col] = 'o'
            direction = random.choice([-1, 1])

            # if not (col + direction >= 0):
            #     direction += 1
            
            # if not (col + direction < cols):
            #     direction += -1
            
            if col + direction >= 0 and col + direction < cols:
                col += direction
                
            row += 1

        last_row[col].append('o')

    number_of_balls = 1500
    for _ in range(number_of_balls):
        initial_col = random.randint(0, cols - 1)
        drop_ball(19)
    
    for row in last_row:
        print(''.join(row))
    
if __name__ == "__main__":
    rows = 50
    cols = 20
    galton_board(rows, cols)
