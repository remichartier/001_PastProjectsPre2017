# implementation of card game - Memory
# http://www.codeskulptor.org/#user24_tIFhMdmxKHOkIGE.py

import simplegui
import random

# initialize constants
CAN_WIDTH = 800
CAN_HEIGHT = 100
# cards are logically 50x100 pixels in size    
CARD_WIDTH = (CAN_WIDTH / 16) 
LINE_WIDTH = 5
CARD_HEIGHT = CAN_HEIGHT
# initialize globals
exposed = []
deck = []
state = 0
nb1 = nb2 = -1
counter = 0

# helper function to initialize globals
def new_game():
    global deck
    global exposed
    global state
    global counter
    counter = 0
    label.set_text("Turns = " + str(counter))
    state = 0
    print "State :" + str(state)
    numbers1 = range(8)
    numbers2 = range(8)
    deck = numbers1 + numbers2
    random.shuffle(deck)
    for i in range(16):
        if len(exposed) == 16:
            exposed[i]=False
        else:
            exposed.append(False)
      
# define event handlers
def mouseclick(pos):
    global nb1, nb2
    global state
    global counter
    # 5.
    nbCard = (pos[0] / CARD_WIDTH) + 1
    if(exposed[nbCard-1] == False):
        print "Click on Card #" + str(nbCard)
        # add game state logic here
        if state == 0:
            # 6.
            exposed[nbCard-1] = True
            nb1 = nbCard
            state = 1
        elif state == 1:
            exposed[nbCard-1] = True
            nb2 = nbCard
            state = 2
            counter = counter +1
            label.set_text("Turns = " + str(counter))            
            print "State :" + str(state)
            print "2 last numbers clicked :" + str(deck[nb1-1]) + " "+ str(deck[nb2-1])
        else:
            if (deck[nb1-1]!=deck[nb2-1]):
                exposed[nb1-1]=False
                exposed[nb2-1]=False
            exposed[nbCard-1] = True
            nb1=nbCard
            state = 1
            print "State :" + str(state)
    
    
# cards are logically 50x100 pixels in size    
def draw(canvas):  
    point = [50/3, 2*100/3]
    for i in range(16): 
        if exposed[i] == True:
            canvas.draw_text(str(deck[i]), point, 40, "White")
        else:
            #draw green rectangle
            x1 = (i * CARD_WIDTH)  
            y1 = 0
            x2 = x1 + CARD_WIDTH # y2 = y1 # x3 = x2
            y3 = CARD_HEIGHT # x4 = x1 # y4 = y3
            canvas.draw_polygon([[x1,y1],[x2,y1],[x2,y3],[x1,y3]],LINE_WIDTH,"Black","Green")
        point[0] += 50

# create frame and add a button and labels

#frame = simplegui.create_frame("Memory", 800, 100)
frame = simplegui.create_frame("Memory", CAN_WIDTH, CAN_HEIGHT)

frame.add_button("Restart", new_game)
label = frame.add_label("Turns = 0")

# register event handlers
frame.set_mouseclick_handler(mouseclick)
frame.set_draw_handler(draw)

# get things rolling
new_game()
frame.start()


# Always remember to review the grading rubric
