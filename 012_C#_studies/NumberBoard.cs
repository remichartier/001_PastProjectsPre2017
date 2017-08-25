using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace GameProject
{
    /// <remarks>
    /// Class that encapsulates the board of number tiles to guess
    /// </remarks>
    class NumberBoard
    {
        #region Fields

        const int BORDER_SIZE = 8;
        const int NUM_COLUMNS = 3;
        const int NUM_ROWS = NUM_COLUMNS;

        // drawing support
        Texture2D boardTexture;
        Rectangle drawRectangle;

        // side length for each tile
        int tileSideLength;

        // tiles
        NumberTile[,] tiles = new NumberTile[NUM_ROWS, NUM_COLUMNS];
   
        
        

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="contentManager">the content manager</param>
        /// <param name="center">the center of the board</param>
        /// <param name="sideLength">the side length for the board</param>
        /// <param name="correctNumber">the correct number</param>
        /// <param name="soundBank">the sound bank for sound effects</param>
        public NumberBoard(ContentManager contentManager, Vector2 center, int sideLength, 
            int correctNumber, SoundBank soundBank)
        {
            // 16 . Write the NumberBoard constructor, which loads the content (by calling the LoadContent method you
                    //wrote in the previous step), creates a new draw rectangle object, 
                    //and calculates the size of the number tiles
                    //on the board. Be sure to include borders around the tiles using the BORDER_SIZE constant; the picture
                    //below might help you generate the appropriate calculation. Don’t create the number tiles yet (Chapter 5,
                    //Week 2)
            
            // load content for the board and create draw rectangle
            this.LoadContent(contentManager);
            drawRectangle = new Rectangle((int)center.X -sideLength/2, (int) center.Y -sideLength/2, sideLength, sideLength);
            // calculate side length for number tiles
            tileSideLength = (sideLength - ((NUM_COLUMNS + 1) * BORDER_SIZE)) / NUM_COLUMNS;

            // initialize array of number tiles
            // 24. In the NumberBoard constructor, add a nested for loop to initialize the NumberTile array. For now, I
                    //just create all the tiles with 8 for the tile number and correct number arguments and null for the sound
                    //bank argument. Note that we also have to calculate the center for each number tile to pass in to the
                    //NumberTile constructor. Use the CalculateTileCenter helper method I provided to help with this
                    //(Chapter 10, Week 5)
            // 28. Change the calls to the NumberTile constructor to use the actual numbers for each of the tiles. The most
                //intuitive way to do this is to use a counter variable that keeps track of the current tile number, but you can
                //also do this by calculating the current tile number based on the loop control variables for the nested for
                //loops. The board should now contain the correct number tiles when you run the game (Chapter 10, Week 5)
            int k = 1;
            for(int i=0;i<NUM_ROWS;i++)
            {
                
                for (int j = 0; j < NUM_COLUMNS; j++)
                {

                    //65. In the NumberBoard constructor, change the call to the NumberTile constructor to pass the
                    //    soundBank parameter as the sound bank argument (instead of null) (Chapter 4, Week 2)

                   tiles[i,j] = new NumberTile(contentManager,
                       CalculateTileCenter(i,j), tileSideLength, k, correctNumber, soundBank);
                   k++;
                }
            }


        }

        #endregion

        #region Public methods

        /// <summary>
        /// Updates the board based on the current mouse state. The only required action is to identify
        /// that the left mouse button has been clicked and update the state of the appropriate number
        /// tile.
        /// </summary>
        /// <param name="gameTime">the current GameTime</param>
        /// <param name="mouse">the current mouse state</param>
        /// <return>true if the correct number was guessed, false otherwise</return>
        public bool Update(GameTime gameTime, MouseState mouse)
        {
            // update all the number tiles
            // 30. In the NumberBoard Update method, add code to call the Update method for each of the tiles. Using
            //    nested for loops is a reasonable way to do this (Chapter 10, Week 5)
            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLUMNS; j++)
                {

                    //52. In the NumberBoard Update method, we need to return true if one of the NumberTile Update
                    //    calls returns true (indicating that the correct number was guessed), 
                    //    otherwise we need to return false.
                    //    Change the call to the NumberTile Update method to put the result 
                    //    into a bool variable; the syntax is
                    //    similar to when you called the Deck TakeTopCard method 
                    //    and put the returned value into a Card
                    //    variable (in Lab 4). If this bool variable is true after the call to the method, 
                    //    immediately return true
                    //    from the NumberBoard Update method. Doing it this way ensures the method immediately returns
                    //    true if the correct number is guessed and also lets us skip the updates for the rest 
                    //    of the number tiles.
                    //    There are more complicated ways to do this to make sure all the tiles get updated 
                    //    (to make sure we don’t
                    //    skip updating shrinking animations for 1/60 of a second, for example, for tiles that 
                    //    are currently shrinking),
                    //    but this approach works fine here (Chapter 13, Week 6 for method return)
                    if (tiles[i, j].Update(gameTime, mouse))
                        return true;

                }
            }
            // return false because the correct number wasn't guessed
            return false;
        }

        /// <summary>
        /// Draws the board
        /// </summary>
        /// <param name="spriteBatch">the SpriteBatch to use for the drawing</param>
        public void Draw(SpriteBatch spriteBatch)
        {
            // draw the board
            // 17. Write the NumberBoard Draw method to draw the board texture in the board draw rectangle. 
                // Don’t draw the number tiles yet. (Chapter 5, Week 2)
            spriteBatch.Draw(boardTexture, drawRectangle, Color.White);

            // draw all the number tiles
            // 26. In the NumberBoard Draw method, add code to draw all the tiles. 
                // Using nested for loops is a reasonable way to do this (Chapter 10, Week 5)
            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLUMNS; j++)
                {
                    tiles[i, j].Draw(spriteBatch);

                }
            }
            
        }

        #endregion

        #region Private methods

        /// <summary>
        /// Loads the content for the board
        /// </summary>
        /// <param name="contentManager">the content manager</param>
        private void LoadContent(ContentManager contentManager)
        {
            // load the background for the board
            // 15. Write the NumberBoard LoadContent method (Chapter 5, Week 2)
            boardTexture = contentManager.Load<Texture2D>("board");
//            drawRectangle = new Rectangle(0, 0, boardTexture.Width / 2, boardTexture.Height / 2);


        }

        /// <summary>
        /// Calculates the center of the tile at the given row and column
        /// </summary>
        /// <param name="row">the row in the array</param>
        /// <param name="column">the column in the array</param>
        /// <returns>the center of the tile in the given row and column</returns>
        private Vector2 CalculateTileCenter(int row, int column)
        {
            int upperLeftX = drawRectangle.X + (BORDER_SIZE * (column + 1)) + 
                tileSideLength * column;
            int upperLeftY = drawRectangle.Y + (BORDER_SIZE * (row + 1)) + 
                tileSideLength * row;
            return new Vector2(upperLeftX + tileSideLength / 2,
                upperLeftY + tileSideLength / 2);
        }

        #endregion
    }
}
