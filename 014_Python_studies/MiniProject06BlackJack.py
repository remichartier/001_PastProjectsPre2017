# Mini-project #6 - Blackjack
#http://www.codeskulptor.org/#user26_NcLQ7Ccmbx_182.py

import simplegui
import random

CANVA_WIDTH = 600
CANVA_HEIGHT = CANVA_WIDTH
# load card sprite - 949x392 - source: jfitz.com
CARD_SIZE = (73, 98)
CARD_CENTER = (36.5, 49)
card_images = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/cards.jfitz.png")

CARD_BACK_SIZE = (71, 96)
CARD_BACK_CENTER = (35.5, 48)
card_back = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/card_back.png")    

# initialize some useful global variables
in_play = False
outcome = ""
outcome2 = ""
outcomeColor = 'Red'
score = 0



# define globals for cards
SUITS = ('C', 'S', 'H', 'D')
RANKS = ('A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K')
VALUES = {'A':1, '2':2, '3':3, '4':4, '5':5, '6':6, '7':7, '8':8, '9':9, 'T':10, 'J':10, 'Q':10, 'K':10}


# define card class
class Card:
    def __init__(self, suit, rank):
        if (suit in SUITS) and (rank in RANKS):
            self.suit = suit
            self.rank = rank
        else:
            self.suit = None
            self.rank = None
            print "Invalid card: ", suit, rank

    def __str__(self):
        return self.suit + self.rank

    def get_suit(self):
        return self.suit

    def get_rank(self):
        return self.rank

    def draw(self, canvas, pos):
        card_loc = (CARD_CENTER[0] + CARD_SIZE[0] * RANKS.index(self.rank), 
                    CARD_CENTER[1] + CARD_SIZE[1] * SUITS.index(self.suit))
        canvas.draw_image(card_images, card_loc, CARD_SIZE, [pos[0] + CARD_CENTER[0], pos[1] + CARD_CENTER[1]], CARD_SIZE)
        
# define hand class
class Hand:
    def __init__(self): # create Hand object
        self.hand = []	

    def __str__(self):
        # return a string representation of a hand
        s = "Hand contains "
        for i in range(len(self.hand)):
            s += str(self.hand[i]) + " "
        return s

    def add_card(self, card):
        # add a card object to a hand
        self.hand.append(card)

    def get_value(self):	# compute the value of the hand, see Blackjack video
        # count aces as 1, if the hand has an ace, then add 10 to hand value if it doesn't bust
        v = 0
        countAce = 0
        for card in self.hand:
            if (card.rank == 'A'):
                countAce += 1	
            v += VALUES[card.rank]
        #if hand contains Ace
        if countAce > 0 :
             if v + 10 <= 21 :
                v += 10
        return v;  
    
    def printhand(self, label):
        h = self.hand
        # print label + " : " + str(self) + "value = " + str(self.get_value())
        
            
    def draw(self, canvas, pos):
        # draw a hand on the canvas, use the draw method for cards
        i=0
        for card in self.hand:
            if i<5:
                card.draw(canvas, pos)
                pos[0] += CANVA_WIDTH/6
            i += 1
            

        
# define deck class 
class Deck:
    def __init__(self):
        self.deck = []	# create a Deck object
        # initialize the deck with all cards from all
        # suits and all ranks.
        for suit in SUITS:
            for rank in RANKS:
                self.deck.append(Card(suit,rank))
        

    def shuffle(self):
        # shuffle the deck 
        random.shuffle(self.deck)    # use random.shuffle()

    def deal_card(self):
        return self.deck.pop()	# deal a card object from the deck
    
    def __str__(self):
        s = ""	# return a string representing the deck
        s += "Deck contains "
        for card in self.deck:
            s += str(card) + " "
        return s



#define event handlers for buttons
def deal():
    #  deck (stored as a global variable), create new player 
    # and dealer hands (stored as global variables)
    global outcome, outcomeColor, outcome2, in_play, deck, player_hand, score, dealer_hand
    outcome = "NEW GAME starts !" 
    outcome2 = "Push HIT or STAND button ?"   
    outcomeColor = 'Aqua'
    # your code goes here
    if in_play == True :
        outcome = "Round interrupted before win --> You Lost !"
        outcome2 = "NEW GAME starts ! Push HIT or STAND button ?"
        score -= 1
    player_hand = Hand()
    dealer_hand = Hand()
    deck = Deck()
    deck.shuffle()
    for i in range(0,2):
        player_hand.add_card(deck.deal_card())
        dealer_hand.add_card(deck.deal_card())
    # player_hand.printhand("Player") #print "Player : " + str(player_hand)
    # dealer_hand.printhand("Dealer") #print "Dealer : " + str(dealer_hand)
    in_play = True

def hit():
    global outcomeColor, outcome, outcome2, in_play, score
    # if the hand is in play, hit the player
    if in_play == False:
        outcome = "Game Over - Game Over - Game Over"
        outcome2 = "Push DEAL button to start again."     
        return
    if player_hand.get_value() < 21:
        player_hand.add_card(deck.deal_card())
        player_hand.printhand("Player")    
    # if busted, assign a message to outcome, update in_play and score
    if player_hand.get_value()>21:
       outcomeColor = 'Red'
       outcome = ">21 ===> You busted. You LOST !"
       outcome2 = "Push DEAL button to start again."  
       in_play = False
       score -= 1
    elif player_hand.get_value() == 21 :
        outcomeColor = 'Aqua'
        outcome = "21 ==> YOU WON !!!"     
        outcome2 = "Start a new game, push DEAL Button !"
        in_play = False
        score += 1
    else:
        outcomeColor = 'Black'
        outcome = ""
        outcome2 = "Push HIT or STAND button ?"   

def stand():
    global outcome, outcome2, outcomeColor, in_play, score
    if in_play == False:
        outcome = "Game Over - Game Over - Game Over"
        outcome2 = "Push DEAL button to start again."     
        return
    in_play = False
    if player_hand.get_value()>21:
        outcomeColor = 'Red'
        outcome = "No more !. You busted !!!! You already LOST !" 
        outcome2 = "Push DEAL button to start again. !"     
    # if hand is in play, repeatedly hit dealer until his hand has value 17 or more
    else :
        outcomeColor = 'Black'
        outcome =  "Now, dealer hits"
        outcome2 = ""
        while dealer_hand.get_value() <= 17 :
            dealer_hand.add_card(deck.deal_card())
            dealer_hand.printhand("Dealer")
        if dealer_hand.get_value() > 21 :
            outcomeColor = 'Aqua'
            outcome = "Dealer busted !!!! YOU WON !!!"     
            outcome2 = "Start a new game, push DEAL Button !"
            score += 1
        else :
            if (player_hand.get_value() <= dealer_hand.get_value()):
                outcomeColor = 'Red'
                outcome =  "Dealer >= Player ===> DEALER WINS !!!"
                outcome2 = "Push DEAL button to start again. !"
                score -= 1
            else :
                outcomeColor = 'Aqua'
                outcome = "Player hand > Dealer hand ===> YOU WON !!!" 
                outcome2 = "Push DEAL button to start again. !"
                score += 1
                
        # assign a message to outcome, update in_play and score

# draw handler    
def draw(canvas):
    # test to make sure that card.draw works, replace with your code below
    #    card = Card("S", "A")
    #    card.draw(canvas, [300, 300])
    player_hand.draw(canvas,[CANVA_WIDTH/12, 2*CANVA_HEIGHT/3])
    dealer_hand.draw(canvas,[CANVA_WIDTH/12, CANVA_HEIGHT/3])
    if in_play == True :
        # back of a card (provided in the template) over the dealer's first (hole) card to hide it.
        #card_back
        #CARD_BACK_SIZE = (71, 96)
        #CARD_BACK_CENTER = (35.5, 48)
        #canvas.draw_image(image, center_source, width_height_source, center_dest, width_height_dest)
        canvas.draw_image(card_back, CARD_BACK_CENTER, CARD_BACK_SIZE, [CANVA_WIDTH/12 + CARD_BACK_CENTER[0], CANVA_HEIGHT/3 + CARD_BACK_CENTER[1]], CARD_BACK_SIZE)
        canvas.draw_text("Dealer's Hand : " , [CANVA_WIDTH/12, 7*CANVA_HEIGHT/24] , 40, 'Black')
    else:
        canvas.draw_text("Dealer's Hand : " + str(dealer_hand.get_value()), [CANVA_WIDTH/12, 7*CANVA_HEIGHT/24] , 40, 'Black')
    canvas.draw_text("Blackjack", [CANVA_WIDTH/12, CANVA_HEIGHT/6] , 60, 'Aqua')
    canvas.draw_text("Dealer's Hand : ", [CANVA_WIDTH/12, 7*CANVA_HEIGHT/24] , 40, 'Black')
    canvas.draw_text("Player's Hand : " + str(player_hand.get_value()), [CANVA_WIDTH/12, 15*CANVA_HEIGHT/24] , 40, 'Black')
    canvas.draw_text(outcome, [CANVA_WIDTH/24, 22* CANVA_HEIGHT/24], 20, outcomeColor)
    canvas.draw_text(outcome2, [CANVA_WIDTH/24, 23* CANVA_HEIGHT/24], 20, outcomeColor)
    canvas.draw_text("Score : " + str(score), [8*CANVA_WIDTH/12, CANVA_HEIGHT/6] , 30, 'Black')
    # initialization frame
#frame = simplegui.create_frame("Blackjack", 600, 600)
frame = simplegui.create_frame("Blackjack", CANVA_WIDTH, CANVA_HEIGHT)

frame.set_canvas_background("Green")

#create buttons and canvas callback
frame.add_button("Deal", deal, 200)
frame.add_button("Hit",  hit, 200)
frame.add_button("Stand", stand, 200)
frame.set_draw_handler(draw)


# get things rolling
deal()
frame.start()


# remember to review the gradic rubric
