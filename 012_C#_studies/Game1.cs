using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

namespace GameProject
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        // 1. Add code to the Game1 constructor to set the window width to 800 
        //  and height to 600 and to make the mouse visible (Chapter 5, Week 2)
        const int WINDOW_WIDTH = 800;
        const int WINDOW_HEIGHT = 600;

        // 2. Add the openingscreen.png file to the GameProjectContent project. 
        //  This is the graphical asset that will be
        //  displayed when the game starts (Chapter 5, Week 2)---> Done.

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        
        //58. At this point, our game always has 8 as the correct number. Declare a Random field in the Game1 class
        //    and use new Random() to instantiate the object for the field (Chapter 12, Week 6)
            
            Random rand = new Random();

        // 62. Declare fields in the Game1 class for the audio api components (see Chapter 14 in the book) (Chapter 14,
        //    Week 7)

            AudioEngine audioEngine;
            WaveBank waveBank;
            SoundBank soundBank;

        // game state
        // This field is used to keep track of the current game state
        GameState gameState = GameState.Menu;

        // Increment 1: opening screen fields
        // 4. Declare a Texture2D field in the Game1 class to hold the opening screen image. As usual, we’ll load
        //      content into this field then draw it as appropriate (Chapter 5, Week 2)
        Texture2D openingScreenImgText2D;

        // 5. Declare a Rectangle field in the Game1 class to hold the draw rectangle for the opening screen image.
        Rectangle drawRectangle;

 
        // Increment 2: the board
        // 11. Declare a NumberBoard field in the Game1 class. 
            // This field will be used to hold the number board for the current game (Chapter 4, Week 2)
        NumberBoard numberBoard;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Increment 1: set window resolution
            graphics.PreferredBackBufferWidth = WINDOW_WIDTH;
            graphics.PreferredBackBufferHeight = WINDOW_HEIGHT;

            // Increment 1: make the mouse visible
            this.IsMouseVisible = true;
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            base.Initialize();


        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // load audio content
            // 63. In the Game1 LoadContent method, add code to load the audio content (see Chapter 14 in the book)
            //    (Chapter 14, Week 7)
            audioEngine = new AudioEngine(@"Content\sounds.xgs");
            waveBank = new WaveBank(audioEngine,@"Content\Wave Bank.xwb");
            soundBank = new SoundBank(audioEngine,@"Content\Sound Bank.xsb");
 

            // Increment 1: load opening screen and set opening screen draw rectangle
            // 4. Declare a Texture2D field in the Game1 class to hold the opening screen image. As usual, we’ll load
            //      content into this field then draw it as appropriate (Chapter 5, Week 2)
            // Texture2D openingScreenImgText2D;
            // 5. In the Game1 LoadContent method, load the opening screen texture into the field from Step 4 (Chapter
            //  5, Week 2)
            // 7. In the Game1 LoadContent method, create a new Rectangle object and put it in the field from Step
            //        5. The rectangle should be created so the opening screen takes up the entire window (Chapter 5, Week 2)
            openingScreenImgText2D = Content.Load<Texture2D>("openingscreen");
            drawRectangle = new Rectangle(0, 0, graphics.PreferredBackBufferWidth, graphics.PreferredBackBufferHeight);

            // Increment 2: create the board object (this will be moved before you're done with the project)
            // 12. In the Game1 LoadContent method, create a new NumberBoard object and put it in the field from
                    //Step 11. Use the window width and height to calculate the board side length and the board center before
                    //calling the constructor. The board should be centered in the window and should be smaller than the window
                    //width and height. At this point, I just set the correct number to 8 and the sound bank to null
                    //(Chapter 4, Week 2)
            // Calculate first the board side length and the board center Use the window width and height
            
            //55. Cut the code you added to the Game1 LoadContent method to calculate the board size and actually
            //    create the board and paste that code into the StartGame method I provided at the end of the Game1
            //    class. Call the StartGame method from the Game1 LoadContent method where you used to have that
            //        code. You'll see why we need to do this soon (Chapter 4, Week 2)

            // create the board object (this will be moved before you're done)
            //int sideLength = 0;
            //if (graphics.PreferredBackBufferWidth < graphics.PreferredBackBufferHeight)
            //{
            //    sideLength = graphics.PreferredBackBufferWidth;
            //}
            //else
            //{
            //    sideLength = graphics.PreferredBackBufferHeight;
            //}
            //Vector2 center = new Vector2((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));
            //            numberBoard = new NumberBoard(Content, center, (int)(sideLength * 0.8), 8, null);
            this.StartGame();
 
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            // 70. Change the if statement at the top of the Game1 Update method to exit if the <Esc> key is pressed
            // (Chapter 15, Week 7)
            if ((GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed) || 
                (Keyboard.GetState().IsKeyDown(Keys.Escape)))
                this.Exit();

            
          

            // Increment 2: change game state if game state is GameState.Menu and user presses Enter
            // 18. In the Game1 Update method, add an if statement that changes the game state to GameState.Play if
                    //the current game state is GameState.Menu and the Enter key is pressed. The following code returns
                    //true if the Enter key is pressed and false otherwise:
                    //Keyboard.GetState().IsKeyDown(Keys.Enter)
                    //and the following code changes the game state to GameState.Play
                    //gameState = GameState.Play;
                    //(Chapter 7, Week 3)
            if ((gameState == GameState.Menu) && (Keyboard.GetState().IsKeyDown(Keys.Enter)))
                gameState = GameState.Play ; 

            // if we're actually playing, update mouse state and update board
            // 29. In the Game1 Update method, if the current game state is GameState.Play get the current mouse            
                    // state and call the NumberBoard Update method (Chapter 8, Week 4)
            if (gameState == GameState.Play)
            {
                MouseState mouse = Mouse.GetState();

                //54. In the Game1 Update method, change the call to the NumberBoard Update method to declare a
                //    variable to tell whether or not the correct number has been guessed and put the returned 
                //    value from the
                //    method call into that variable (Chapter 4, Week 2)
                bool guessed = numberBoard.Update(gameTime, mouse);

                //56. In the Game1 Update method, add an if statement right after the call to the NumberBoard Update
                //    method to check if the correct number has been guessed. If it has, call the StartGame method (Chapter 7,
                //    Week 3)
                if (guessed)
                {
                    //69. In the Game1 Update method, add code to play the newGame cue before calling the StartGame
                    //    method. We do this here rather than in the StartGame method because we don't want this cue to play for
                    //    the first game (Chapter 14, Week 7)
                    soundBank.PlayCue("correctGuess"); 
                    this.StartGame();

                }
            }


            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // Increments 1 and 2: draw appropriate items here

            // 4. Declare a Texture2D field in the Game1 class to hold the opening screen image. As usual, we’ll load
            //      content into this field then draw it as appropriate (Chapter 5, Week 2)
            // 8. In the Game1 Draw method, draw the opening screen texture in the opening screen draw rectangle
            //    (Chapter 5, Week 2)
            spriteBatch.Begin();
            
            // 10. In the Game1 Draw method, add an if statement that checks if the current game state is
                    // GameState.Menu before drawing the opening screen texture in the opening screen draw rectangle
                    // (Chapter 7, Week 3)
                    // This step uses something called an enumeration, which we haven't discussed yet. You can read
                    // about enumerations in Section 8.1 of the book if you'd like, but for this step you just need to know that
                    // gameState == GameState.Menu
                    // is a Boolean expression that will evaluate to true if the value of the gameState variable is currently
                    // GameState.Menu.
            if (gameState == GameState.Menu)
                spriteBatch.Draw(openingScreenImgText2D, drawRectangle, Color.White);
            else
            {
                // 19. In the Game1 Draw method, add an else clause to the if statement from Step 10 to have the board draw
                    //  itself if the current game state is GameState.Play
                    //  (Chapter 7, Week 3)
                numberBoard.Draw(spriteBatch);

            }
            spriteBatch.End();

            base.Draw(gameTime);
        }

        /// <summary>
        /// Starts a game
        /// </summary>
        void StartGame()
        {
            // randomly generate new number for game
            //59. In the StartGame method, declare a variable to hold the correct number for the current game and set it to
            //    a random number (from 1 to 9 inclusive) you generate using the field from the previous step. Pass that
            //    number as the correct number argument when you call the NumberBoard constructor in the StartGame
            //    method (Chapter 4, Week 2)
            int number = rand.Next(1, 10);



            //55. Cut the code you added to the Game1 LoadContent method to calculate the board size and actually
            //    create the board and paste that code into the StartGame method I provided at the end of the Game1
            //    class. Call the StartGame method from the Game1 LoadContent method where you used to have that
            //        code. You'll see why we need to do this soon (Chapter 4, Week 2)

            // create the board object (this will be moved before you're done)
            int sideLength = 0;
            if (graphics.PreferredBackBufferWidth < graphics.PreferredBackBufferHeight)
            {
                sideLength = graphics.PreferredBackBufferWidth;
            }
            else
            {
                sideLength = graphics.PreferredBackBufferHeight;
            }
            Vector2 center = new Vector2((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));
            
            // 64. In the StartGame method, pass the sound bank as the sound bank argument to the NumberBoard
            //    constructor (instead of the null you’ve been passing) (Chapter 4, Week 2)

            numberBoard = new NumberBoard(Content, center, (int)(sideLength * 0.8), number, soundBank);
        }
    }
}
