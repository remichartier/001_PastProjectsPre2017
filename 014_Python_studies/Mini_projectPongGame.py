# Implementation of classic arcade game Pong
# http://www.codeskulptor.org/#user23_YpSanAigpXojWIB.py

import simplegui
import random

# initialize globals - pos and vel encode vertical info for paddles
WIDTH = 600
HEIGHT = 400       
BALL_RADIUS = 20
PAD_WIDTH = 8
PAD_HEIGHT = 80
HALF_PAD_WIDTH = PAD_WIDTH / 2
HALF_PAD_HEIGHT = PAD_HEIGHT / 2
LEFT = False
RIGHT = True
paddle1_pos=[HALF_PAD_WIDTH, HEIGHT/2]
paddle2_pos=[WIDTH-HALF_PAD_WIDTH,HEIGHT/2] 
paddle1_vel=0
paddle2_vel=0


# initialize ball_pos and ball_vel for new bal in middle of table
# if direction is RIGHT, the ball's velocity is upper right, else upper left
def spawn_ball(direction):
    global ball_pos, ball_vel # these are vectors stored as lists
    rand=random.random()
    yTarget = BALL_RADIUS +(((HEIGHT/2)-BALL_RADIUS)*rand)
    ball_pos = [WIDTH/2, HEIGHT/2]
    # Req 5
    ball_vel=[(random.randrange(120, 240))/60.0, -(random.randrange(60, 180))/60.0]
    if(direction==LEFT):
        ball_vel[0] = -ball_vel[0]

# define event handlers
def new_game():
    global paddle1_pos, paddle2_pos, paddle1_vel, paddle2_vel  # these are numbers
    global score1, score2  # these are ints
    #Req14 :
    score1=score2=0
    spawn_ball(RIGHT)
    
def draw(c):
    global score1, score2, paddle1_pos, paddle2_pos, ball_pos, ball_vel
    
    # draw mid line and gutters
    c.draw_line([WIDTH / 2, 0],[WIDTH / 2, HEIGHT], 1, "White")
    c.draw_line([PAD_WIDTH, 0],[PAD_WIDTH, HEIGHT], 1, "White")
    c.draw_line([WIDTH - PAD_WIDTH, 0],[WIDTH - PAD_WIDTH, HEIGHT], 1, "White")       
        
    # update ball
    ball_pos[0] += ball_vel[0]
    ball_pos[1] += ball_vel[1]
    
    # Req4 : ball collide glutters top or bottom ?
    ball_top = ball_pos[1]-BALL_RADIUS
    ball_bottom = ball_pos[1]+BALL_RADIUS
    if ball_top<=0 or ball_bottom > HEIGHT:
        # inverse ball y speed
        ball_vel[1] *= (-1)
    
    # Req 6 : collide glutter Left or Right ?
    # Req 6 : collide glutter right ?
    if (ball_pos[0]>WIDTH-PAD_WIDTH-BALL_RADIUS):
        # Req11 : if collide right glutter, then check if collide paddel2 as well
        pad_top = paddle2_pos[1]-HALF_PAD_HEIGHT        
        pad_bottom = paddle2_pos[1] + HALF_PAD_HEIGHT
        if (not (ball_top>pad_bottom or ball_bottom<pad_top)):
            print "Collision Paddle Right"        
            #Req12 : increase velocity by 10%.
            #Inverse vel x 
            ball_vel[0] *= -1.1
            ball_vel[1] *= 1.1
            ball_pos[0] -= 1
        else: 
            score1 += 1
            spawn_ball(LEFT)

    # Req 6 : collide glutter left ?
    if(ball_pos[0] <PAD_WIDTH+BALL_RADIUS):
        # Req11 : if collide left glutter, then check if collide paddel1 as well
        pad_top = paddle1_pos[1]-HALF_PAD_HEIGHT        
        pad_bottom = paddle1_pos[1] + HALF_PAD_HEIGHT
        if (not (ball_top>pad_bottom or ball_bottom<pad_top)):
            print "Collision Paddle Left"
            #Req12 : increase velocity by 10%.
            #Inverse vel x 
            ball_vel[0] *= -1.1
            ball_vel[1] *= 1.1
            ball_pos[0] += 1
        else:
            score2 += 1
            spawn_ball(RIGHT)
    
    # draw ball
    c.draw_circle(ball_pos, BALL_RADIUS, 1, "Blue", "Blue")
    
    # update paddle's vertical position, keep paddle on the screen
    # Req8 : Update paddle positions.
    paddle1_pos[1] += paddle1_vel
    paddle2_pos[1] += paddle2_vel
    
    # Req10 : Restrict paddles to upper/lower bounds.
    if(paddle1_pos[1] + HALF_PAD_HEIGHT>HEIGHT):
        paddle1_pos[1] = HEIGHT - HALF_PAD_HEIGHT 
    if(paddle1_pos[1] - HALF_PAD_HEIGHT<0):
        paddle1_pos[1] = HALF_PAD_HEIGHT 
        
    if(paddle2_pos[1] + HALF_PAD_HEIGHT>HEIGHT):
        paddle2_pos[1] = HEIGHT - HALF_PAD_HEIGHT 
    if(paddle2_pos[1] - HALF_PAD_HEIGHT<0):
        paddle2_pos[1] = HALF_PAD_HEIGHT 
    
    # Req7 : draw paddles
    # paddle1
    c.draw_polygon([[paddle1_pos[0]-HALF_PAD_WIDTH,paddle1_pos[1]-HALF_PAD_HEIGHT],
                        [paddle1_pos[0]+HALF_PAD_WIDTH,paddle1_pos[1]-HALF_PAD_HEIGHT],
                        [paddle1_pos[0]+HALF_PAD_WIDTH,paddle1_pos[1]+HALF_PAD_HEIGHT],
                        [paddle1_pos[0]-HALF_PAD_WIDTH,paddle1_pos[1]+HALF_PAD_HEIGHT]], 1, "White", "White")

    # paddle2
    c.draw_polygon([[paddle2_pos[0]-HALF_PAD_WIDTH,paddle2_pos[1]-HALF_PAD_HEIGHT],
                        [paddle2_pos[0]+HALF_PAD_WIDTH,paddle2_pos[1]-HALF_PAD_HEIGHT],
                        [paddle2_pos[0]+HALF_PAD_WIDTH,paddle2_pos[1]+HALF_PAD_HEIGHT],
                        [paddle2_pos[0]-HALF_PAD_WIDTH,paddle2_pos[1]+HALF_PAD_HEIGHT]], 1, "White", "White")

    # draw scores
    c.draw_text(str(score1), [1*WIDTH/4, 1*HEIGHT/8], 40, "White")
    c.draw_text(str(score2), [3*WIDTH/4, 1*HEIGHT/8], 40, "White")
    
    
def keydown(key):
    global paddle1_vel, paddle2_vel
    #Req9
    vel=8
    if key == simplegui.KEY_MAP["up"]:
        paddle2_vel -= vel
    elif key == simplegui.KEY_MAP["down"]:
        paddle2_vel += vel
    elif key == simplegui.KEY_MAP["w"]:
        paddle1_vel -= vel
    elif key == simplegui.KEY_MAP["s"]:    
        paddle1_vel += vel
        
        
def keyup(key):
    global paddle1_vel, paddle2_vel
    #Req9
    if key == simplegui.KEY_MAP["up"]:
        paddle2_vel = 0 
    elif key == simplegui.KEY_MAP["down"]:
        paddle2_vel = 0
    elif key == simplegui.KEY_MAP["w"]:
        paddle1_vel = 0
    elif key == simplegui.KEY_MAP["s"]:    
        paddle1_vel = 0

def button_handler():
    new_game()

        
# create frame
frame = simplegui.create_frame("Pong", WIDTH, HEIGHT)
frame.set_draw_handler(draw)
frame.set_keydown_handler(keydown)
frame.set_keyup_handler(keyup)
#Req14 :
button1 = frame.add_button('Restart', button_handler)        


# start frame
random.seed()
new_game()
frame.start()


def rpsls(name): 
    # fill in your code below

    # convert name to player_number using name_to_number
    player_number = name_to_number(name)
    # compute random guess for comp_number using random.randrange()
    comp_number = random.randrange(0,5)
    # compute difference of player_number and comp_number modulo five
    difference = (comp_number - player_number) % 5
    # use if/elif/else to determine winner
    if (difference == 1 or difference == 2) :
        winner = "Computer"
    elif (difference == 3 or difference == 4) :
        winner = "Player"
    
    # convert comp_number to name using number_to_name
    
    # print results
    print
    print "Player chooses", number_to_name(player_number)
    print "Computer chooses", number_to_name(comp_number)
    if (difference !=0 ) :
        print winner, "wins!"
    else :
        print "Player and computer tie!"
        


    
# test your code
rpsls("rock")
rpsls("Spock")
rpsls("paper")
rpsls("lizard")
rpsls("scissors")

# always remember to check your completed program against the grading rubric



