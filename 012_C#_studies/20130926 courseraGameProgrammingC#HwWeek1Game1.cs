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

namespace ProgrammingAssignment2
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const int WINDOW_WIDTH = 800;
        const int WINDOW_HEIGHT = 600;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        // STUDENTS: add your sprite variables here
        // 5. Add variables to hold five Texture2D sprites near the top of the Game1 class
        Texture2D smurf0;
        Rectangle drawRectangle0;
        Texture2D smurf1;
        Rectangle drawRectangle1;
        Texture2D smurf2;
        Rectangle drawRectangle2;
        Texture2D smurf3;
        Rectangle drawRectangle3;
        Texture2D smurf4;
        Rectangle drawRectangle4;
        // 6. Run your program to make sure it compiles and runs
        // Result : OK


        // used to handle generating random values
        Random rand = new Random();
        const int CHANGE_DELAY_TIME = 1000;
        int elapsedTime = 0;

        // used to keep track of current sprite and location
        Texture2D currentSprite;
        Rectangle drawRectangle = new Rectangle();

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            graphics.PreferredBackBufferWidth = WINDOW_WIDTH;
            graphics.PreferredBackBufferHeight = WINDOW_HEIGHT;
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

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

            // STUDENTS: load the images here
            // 7. Load the sprites in the LoadContent method 
            //      and change the code as indicated by BOTH the comments
            //      load smurfs and build draw rectangles
            smurf0 = Content.Load<Texture2D>("Baby_smurph");
            drawRectangle0 = new Rectangle(0, 0, smurf0.Width, smurf0.Height);
            smurf1 = Content.Load<Texture2D>("BrainySmurph");
            drawRectangle1 = new Rectangle(0, 0, smurf1.Width, smurf1.Height);
            smurf2 = Content.Load<Texture2D>("papa-smurf");
            drawRectangle2 = new Rectangle(0, 0, smurf2.Width, smurf2.Height);
            smurf3 = Content.Load<Texture2D>("SmurfVanWinkle");
            drawRectangle3 = new Rectangle(0, 0, smurf3.Width, smurf3.Height);
            smurf4 = Content.Load<Texture2D>("The-Smurfs-Episode-24--The-Purple-Smurf");
            drawRectangle4 = new Rectangle(0, 0, smurf4.Width, smurf4.Height);


            // STUDENTS: set the currentSprite variable to one of your sprite variables
            // 7.     and change the code as indicated by BOTH the comments
            currentSprite = smurf0;

            // 8.Run your program to make sure it compiles and runs
            // Result --> Compiles and runs ok.
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
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            elapsedTime += gameTime.ElapsedGameTime.Milliseconds;
            if (elapsedTime > CHANGE_DELAY_TIME)
            {
                elapsedTime = 0;

                // 12. Modify the code in the Update method as indicated by the FIRST TWO comments. Don't do the rest yet.
                // STUDENTS: uncomment the code below and make it generate a random number between 0 and 4
                // using the rand field I provided
                int spriteNumber = rand.Next(5);

                // 12. Modify the code in the Update method as indicated by the FIRST TWO comments. Don't do the rest yet.
                // sets current sprite
                // STUDENTS: uncomment the lines below and change sprite0, sprite1, sprite2, sprite 3, and sprite 4
                //      to the five different names of your sprite variables



                if (spriteNumber == 0)
                {
                    currentSprite = smurf0;
                }
                else if (spriteNumber == 1)
                {
                    currentSprite = smurf1;
                }
                else if (spriteNumber == 2)
                {
                    currentSprite = smurf2;
                }
                else if (spriteNumber == 3)
                {
                    currentSprite = smurf3;
                }
                else if (spriteNumber == 4)
                {
                    currentSprite = smurf4;
                }

                // 13. Run your program to make sure it compiles, runs, and draws a sprite in the upper left corner. 
                //  The sprite should change approximately every second
                // Result : Ok.

                // 14. Modify the code in the Update method as indicated by the THIRD comment. Don't do the rest yet.
                // STUDENTS: uncomment the line below to set drawRectangle.X to a random number between 0 
                //      and the preferred backbuffer width - the width of the current sprite 
                // using the rand field I provided
                drawRectangle.X = rand.Next(graphics.PreferredBackBufferWidth - currentSprite.Width);

                // 15. Run your program to make sure it compiles, runs, and draws a sprite. 
                //  The sprite and the x location of the sprite should change approximately every second. 
                //  The sprite should always be completely in the window
                // Result : OK.

                // 16. Modify the code in the Update method as indicated by the FOURTH comment.
                // STUDENTS: uncomment the line below to set drawRectangle.Y to a random number 
                //      between 0 and the preferred backbuffer height - the height of the current sprite
                // using the rand field I provided
                drawRectangle.Y = rand.Next(graphics.PreferredBackBufferHeight - currentSprite.Height);

                // 18. Run your program to make sure it compiles, runs, and draws a sprite. 
                //      The sprite and the location of the sprite should change approximately every second. 
                //      The sprite should always be completely in the window.
                // Result : OK.

                // 19. You're done!

                // STUDENTS: set the drawRectangle.Width and drawRectangle.Height to match the width and height of currentSprite
                // 9. Modify the code in the Update method as indicated by the LAST comment; don’t do the rest yet
                drawRectangle.Width = smurf0.Width;
                drawRectangle.Height = smurf0.Height;
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

            // STUDENTS: draw current sprite here
            // 10. Add code to the Draw method to draw currentSprite using drawRectangle
            spriteBatch.Begin();
            spriteBatch.Draw(currentSprite, drawRectangle, Color.White);
            spriteBatch.End();

            //11. Run your program to make sure it compiles, runs, and draws a sprite in the upper left corner
            // Result : OK.

            base.Draw(gameTime);
        }
    }
}
