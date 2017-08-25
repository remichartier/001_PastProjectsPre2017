# program template for Spaceship
#http://www.codeskulptor.org/#user27_JPZM8OpL2VgFbkt.py

import simplegui
import math
import random

# globals for user interface
WIDTH = 800
HEIGHT = 600
MAX_LIVES = 20
score = 0
lives = MAX_LIVES
time = 0.5
friction = 0.1
started = False
NB_ROCKS = 13
rock_group = set([])
missile_group = set([])
explosion_group = set([])


class ImageInfo:
    def __init__(self, center, size, radius = 0, lifespan = None, animated = False):
        self.center = center
        self.size = size
        self.radius = radius
        if lifespan:
            self.lifespan = lifespan
        else:
            self.lifespan = float('inf')
        self.animated = animated

    def get_center(self):
        return self.center

    def get_size(self):
        return self.size

    def get_radius(self):
        return self.radius

    def get_lifespan(self):
        return self.lifespan

    def get_animated(self):
        return self.animated

    
# art assets created by Kim Lathrop, may be freely re-used in non-commercial projects, please credit Kim
    
# debris images - debris1_brown.png, debris2_brown.png, debris3_brown.png, debris4_brown.png
#                 debris1_blue.png, debris2_blue.png, debris3_blue.png, debris4_blue.png, debris_blend.png
debris_info = ImageInfo([320, 240], [640, 480])
debris_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/debris2_blue.png")

# nebula images - nebula_brown.png, nebula_blue.png
nebula_info = ImageInfo([400, 300], [800, 600])
nebula_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/nebula_blue.f2013.png")

# splash image
splash_info = ImageInfo([200, 150], [400, 300])
splash_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/splash.png")

# ship image
ship_info = ImageInfo([45, 45], [90, 90], 35)
ship_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/double_ship.png")


# missile image - shot1.png, shot2.png, shot3.png
missile_info = ImageInfo([5,5], [10, 10], 3, 50)
missile_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/shot2.png")
random.randint(0, 10)
# asteroid images - asteroid_blue.png, asteroid_brown.png, asteroid_blend.png
asteroid_info = ImageInfo([45, 45], [90, 90], 40)
asteroid_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/asteroid_blue.png")

# animated explosion - explosion_orange.png, explosion_blue.png, explosion_blue2.png, explosion_alpha.png
explosion_info = ImageInfo([64, 64], [128, 128], 17, 24, True)
explosion_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/explosion_alpha.png")

# sound assets purchased from sounddogs.com, please do not redistribute
soundtrack = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/soundtrack.mp3")
missile_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/missile.mp3")
missile_sound.set_volume(.5)
ship_thrust_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/thrust.mp3")
explosion_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/explosion.mp3")

# helper functions to handle transformations
def angle_to_vector(ang):
    return [math.cos(ang), math.sin(ang)]

def dist(p,q):
    return math.sqrt((p[0] - q[0]) ** 2+(p[1] - q[1]) ** 2)


# Ship class
class Ship:
    # global a_missile


    def __init__(self, pos, vel, angle, image, info):
        self.pos = [pos[0],pos[1]]
        self.vel = [vel[0],vel[1]]
        self.thrust = False
        self.angle = angle
        self.angle_vel = 0
        self.image = image
        self.image_center = info.get_center()
        self.image_size = info.get_size()
        self.radius = info.get_radius()
        self.a = self.image_center
       
        
    def draw(self,canvas):
        # canvas.draw_circle(self.pos, self.radius, 1, "White", "White")
        # canvas.draw_circle(self.pos, self.radius, 1, "White", "White")
        #canvas.draw_image(self.image, self.image_center_cur, self.image_size,
        #                  self.pos, self.image_size, self.angle)
        #canvas.draw_image(self.image, [45,45], self.image_size,self.pos, self.image_size, self.angle)
        canvas.draw_image(self.image, self.a, self.image_size,self.pos, self.image_size, self.angle)
        

    def update(self):
        self.angle += self.angle_vel
        forward = angle_to_vector(self.angle)
        if self.thrust==True:
            self.vel[0] += forward[0]
            self.vel[1] += forward[1]
            
        #speed down via friction
        self.vel[0] *= (1 - friction)
        self.vel[1] *= (1 - friction)    
        
        #speed up is up key still pushed.
        self.pos[0] += self.vel[0]
        self.pos[1] += self.vel[1]
        
        # update positions if ship moves out of frame ...
        self.pos[0] %= WIDTH
        self.pos[1] %= HEIGHT
        #
        # build info for ship thrust image
        self.image_center_cur = self.image_center
        #if self.thrust == True:
        #    self.image_center_cur = self.image_center_thrust        
        #print self.image_center
        
    
    def set_angle_vel(self,delta_vel):
        if delta_vel==0:
            self.angle_vel = 0
        else:
            self.angle_vel += delta_vel
        
    def set_thrust(self,b):
        self.thrust = b
        self.a = [45,45]
        if b==True:
            self.a = [135,45]
            ship_thrust_sound.play()
        else :
            ship_thrust_sound.rewind()
            
    def shoot(self):
        factor = 120
        #(self, pos, vel, ang, ang_vel, image, info, sound = None)
        #a_missile = Sprite([2 * WIDTH / 3, 2 * HEIGHT / 3], [-1,1], 0, 0, missile_image, missile_info, missile_sound)
        forward = angle_to_vector(self.angle)
        return Sprite([self.pos[0] + 45*forward[0], self.pos[1]+ 45*forward[1]], 
                           [self.vel[0] + 5*forward[0] ,self.vel[1] + 5*forward[1] ], 
                           self.angle, 0, missile_image, missile_info, True, missile_sound)

    def get_position(self):
        return self.pos
        
    def get_radius(self):
        return self.radius
        
    
# Sprite class
class Sprite:
    def __init__(self, pos, vel, ang, ang_vel, image, info, show, sound = None, ):
        self.pos = [pos[0],pos[1]]
        self.vel = [vel[0],vel[1]]
        self.angle = ang
        self.angle_vel = ang_vel
        self.image = image
        self.image_center = info.get_center()
        self.image_size = info.get_size()
        self.radius = info.get_radius()
        self.lifespan = info.get_lifespan()
        self.animated = info.get_animated()
        self.age = 0
        self.show = show
        if sound:
            sound.rewind()
            sound.play()
        
            
   
    def draw(self, canvas):
        #canvas.draw_circle(self.pos, self.radius, 1, "Red", "Red")
        if self.show:
            if self.animated:
                #(self, center, size, radius = 0, lifespan = None, animated = False)
                canvas.draw_image(self.image, 
                                  [self.image_center[0] + self.age*self.image_size[0],self.image_center[1]], 
                                  self.image_size,self.pos, 
                                  self.image_size, self.angle)
            else:
                canvas.draw_image(self.image, self.image_center, self.image_size,self.pos, self.image_size, self.angle)
            
     
    def update(self):
        if self.show:
            self.angle += self.angle_vel
            #forward = angle_to_vector(self.angle)
            #self.vel[0] += forward[0]
            #self.vel[1] += forward[1]
            #speed down via friction
            #self.vel[0] *= (1 - friction*3)
            #self.vel[1] *= (1 - friction*3)    
            
            #speed up is up key still pushed.
            self.pos[0] += self.vel[0]
            self.pos[1] += self.vel[1]
            
            # update positions if ship moves out of frame ...
            self.pos[0] %= WIDTH
            self.pos[1] %= HEIGHT
        self.age += 1
        if (self.age < self.lifespan):
            return False
        return True
   
    def get_position(self):
        return self.pos
        
    def get_radius(self):
        return self.radius
    
    def collide(self, other_object):
        if (dist(self.pos, other_object.get_position()) < 
            (self.radius + other_object.get_radius())):
            return True
        return False
            
 
                
           
def draw(canvas):
    global time, started, lives, score
    
    # animiate background
    time += 1
    wtime = (time / 4) % WIDTH
    center = debris_info.get_center()
    size = debris_info.get_size()
    canvas.draw_image(nebula_image, nebula_info.get_center(), nebula_info.get_size(), [WIDTH / 2, HEIGHT / 2], [WIDTH, HEIGHT])
    canvas.draw_image(debris_image, center, size, (wtime - WIDTH / 2, HEIGHT / 2), (WIDTH, HEIGHT))
    canvas.draw_image(debris_image, center, size, (wtime + WIDTH / 2, HEIGHT / 2), (WIDTH, HEIGHT))

    # draw ship and sprites
    my_ship.draw(canvas)
    # a_rock.draw(canvas)
    # rock_group.draw(canvas)
    process_sprite_group(rock_group, canvas)
    # a_missile.draw(canvas)
    process_sprite_group(missile_group,canvas)
    process_sprite_group(explosion_group,canvas)
    
    # draw Score and Lives
    canvas.draw_text("Score : " + str(score), [10*WIDTH/12, 1*HEIGHT/20], 30, "White")
    canvas.draw_text("Lives : " + str(lives), [1*WIDTH/48, 1*HEIGHT/20], 30, "White")
    
    # update ship and sprites
    my_ship.update()
    # a_rock.update()
    # rock_group.update()
    # a_missile.update()
    #missile_group.update()
    if( group_collide(rock_group,my_ship) ):
        lives -= 1
    
    score += group_group_collide(rock_group, missile_group)
    
    if((lives == 0) and (len(rock_group) != 0)):
        started = False
        r=set(rock_group)
        for rock in r:
            rock_group.discard(rock)
        missile_sound.set_volume(0)
        soundtrack.set_volume(0)
        missile_sound.set_volume(0)
        ship_thrust_sound.set_volume(0)
 
    
    # draw splash screen if not started
    if not started:
        canvas.draw_image(splash_image, splash_info.get_center(), 
                          splash_info.get_size(), [WIDTH / 2, HEIGHT / 2], 
                          splash_info.get_size())

            
# timer handler that spawns a rock    
def rock_spawner():
    # global a_rock
    global rock_group
    # Sprite(self, pos, vel, ang, ang_vel, image, info, sound = None):
    #a_rock = Sprite([WIDTH / 3, HEIGHT / 3], [1, 1], 0, 0, asteroid_image, asteroid_info)
    if((len(rock_group) < NB_ROCKS) and started):
        a =[random.randint(0,WIDTH), random.randint(0,HEIGHT)]
        if dist(a, my_ship.get_position()) >  (asteroid_info.get_radius() + my_ship.get_radius() + (WIDTH/5)) :
            rock_group.add(Sprite(a,
                              [random.randrange(-1, 1),
                               random.randrange(-1, 1)],
                              random.random()*2.0*math.pi,
                              random.choice([1,-1]) * random.random()*2.0*math.pi/(120),
                              asteroid_image, asteroid_info, True))

def process_sprite_group(group, canvas):
    a=set(group)
    for s in a :
        if(s.update()):
            group.remove(s)
        s.draw(canvas)
        
        

def group_collide(group, other_object):
    global  explosion_group
    a = set(group)
    for b in a:
        if(b.collide(other_object)):
            # pos, vel, ang, ang_vel, image, info, show, sound = None, )
            explosion_group.add(Sprite(b.get_position(), [0,0], 0, 0, explosion_image, explosion_info, True, explosion_sound ));
            group.remove(b)
            return True
    return False

def group_group_collide(group1, group2):
    g1=set(group1)
    nb=0
    for g in g1:
        if(group_collide(group2,g)):
            group1.discard(g)
            nb +=1
    return nb
            


# Key Down Handler
def keydown(key):
    global missile_group
    vel = 2.0*math.pi/(60)
    if key == simplegui.KEY_MAP["left"]:
        my_ship.set_angle_vel(-vel)
    elif key == simplegui.KEY_MAP["right"]:
        my_ship.set_angle_vel(+vel)
    elif key == simplegui.KEY_MAP["up"]:
        my_ship.set_thrust(True)
        #print self.image_center
    elif key == simplegui.KEY_MAP["space"]:
        missile_group.add(my_ship.shoot())




# Key Up Handler
def keyup(key):
    if key == simplegui.KEY_MAP["left"]:
        my_ship.set_angle_vel(0)
    elif key == simplegui.KEY_MAP["right"]:
        my_ship.set_angle_vel(0)
    elif key == simplegui.KEY_MAP["up"]:
        my_ship.set_thrust(False)

# mouseclick handlers that reset UI and conditions whether splash image is drawn
def click(pos):
    global started, lives, score
    center = [WIDTH / 2, HEIGHT / 2]
    size = splash_info.get_size()
    inwidth = (center[0] - size[0] / 2) < pos[0] < (center[0] + size[0] / 2)
    inheight = (center[1] - size[1] / 2) < pos[1] < (center[1] + size[1] / 2)
    if (not started) and inwidth and inheight:
        started = True
        lives = MAX_LIVES
        score = 0
        missile_sound.set_volume(.5)
        soundtrack.set_volume(.5)
        missile_sound.set_volume(.5)
        ship_thrust_sound.set_volume(.5)
       
    
# initialize frame
frame = simplegui.create_frame("Asteroids", WIDTH, HEIGHT)

# initialize ship and two sprites
my_ship = Ship([WIDTH / 2, HEIGHT / 2], [0, 0], 0, ship_image, ship_info)
# Sprite(self, pos, vel, ang, ang_vel, image, info, sound = None):
#a_rock = Sprite([WIDTH / 3, HEIGHT / 3], [1, 1], 0, 0, asteroid_image, asteroid_info)
#a_rock = Sprite([random.randint(0,WIDTH), random.randint(0,HEIGHT)]
#a_rock = Sprite([random.randint(0,WIDTH), random.randint(0,HEIGHT)],[0,0], random.randint(5,15), 2.0*math.pi*random.random(),random.random()*2.0*math.pi/(60), asteroid_image, asteroid_info)
#a_rock = Sprite([random.randint(0,WIDTH), random.randint(0,HEIGHT)],[0,0],random.random()*2.0*math.pi ,random.random()*2.0*math.pi/(120), asteroid_image, asteroid_info, True)
# a_rock = Sprite([random.randint(0,WIDTH), random.randint(0,HEIGHT)],[0,0],random.random()*2.0*math.pi , random.random()*2.0*math.pi/(120),asteroid_image, asteroid_info, True)
# rock_group = Sprite([random.randint(0,WIDTH), random.randint(0,HEIGHT)],[0,0],random.random()*2.0*math.pi , random.random()*2.0*math.pi/(120),asteroid_image, asteroid_info, True)
# rock_group = set([]) 
# a_missile 
# missile_group = Sprite([-WIDTH, -HEIGHT], [0,0], 0, 0, missile_image, 
#                   missile_info, False, missile_sound)

# register handlers
frame.set_draw_handler(draw)
# register key handlers
frame.set_keydown_handler(keydown)
frame.set_keyup_handler(keyup)
frame.set_mouseclick_handler(click)

timer = simplegui.create_timer(1000.0, rock_spawner)

# get things rolling
timer.start()
frame.start()

