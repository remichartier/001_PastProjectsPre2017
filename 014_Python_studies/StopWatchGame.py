# template for "Stopwatch: The Game"
# http://www.codeskulptor.org/#user22_4sHS8sEzAJZVABv.py

import simplegui

# define global variables

counter = 0
nbStops = 0
nbGoldStops = 0
stopped = True

# define helper function format that converts time
# in tenths of seconds into formatted string A:BC.D
def format(t):
    d = t%10
    bc = (t/10) % 60
    a = (t/10)/60
    if bc<10 :
        return(str(a) +":"+"0"+str(bc)+"."+str(d))
    else:
        return(str(a) +":"+str(bc)+"."+str(d))
    
    
# define event handlers for buttons; "Start", "Stop", "Reset"


# define event handler for timer with 0.1 sec interval
def tick():
    global counter
    counter+=1
    
timer = simplegui.create_timer(100, tick)

# Start the frame animation



# define draw handler
def draw(canvas):
    canvas.draw_text(format(counter), (60, 120), 60, "White")
    canvas.draw_text(str(nbGoldStops)+"/"+str(nbStops),(250,20),20, "White")

def buttonStart_handler():
    global stopped
    timer.start()
    stopped = False
    
def buttonStop_handler():    
    global nbStops, nbGoldStops, stopped
    timer.stop()
    if stopped == False :
        nbStops += 1
        if (counter%10) == 0:
            nbGoldStops += 1
        stopped = True
        
    
def buttonReset_handler():
    global counter, nbStops, nbGoldStops, stopped
    timer.stop()
    counter=nbStops= nbGoldStops=0
    stopped = True
    
# create frame
frame = simplegui.create_frame("StopWatch : The Game ", 300, 200)
frame.add_button("Start", buttonStart_handler)
frame.add_button("Stop", buttonStop_handler)
frame.add_button("Reset", buttonReset_handler)
frame.set_draw_handler(draw)

# register event handlers


# start frame
frame.start()
