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
    /// A number tile
    /// </remarks>
    class NumberTile
    {
        #region Fields

        // original length of each side of the tile
        int originalSideLength;

        // whether or not this tile is the correct number
        bool isCorrectNumber;

        // drawing support
        // 40. Declare a Texture2D field in the NumberTile class 
            //  to hold the blinking tile texture (Chapter 12, Week 6 for fields)
        //42. Now we have two textures we need to use for drawing: one for normal and shrinking tiles and the other for
        // blinking tiles. That means we’ll need to keep track of the current texture we’re using so we draw the
        // appropriate one. Declare a Texture2D field in the NumberTile class to hold the current texture we’ll
        // be drawing (Chapter 12, Week 6)
        Texture2D texture, blinkingTileTexture2D, currentTexture;
        Rectangle drawRectangle;
        Rectangle sourceRectangle;

 

        // blinking support
        const int TOTAL_BLINK_MILLISECONDS = 4000;
        int elapsedBlinkMilliseconds = 0;
        const int FRAME_BLINK_MILLISECONDS = 125;
        int elapsedFrameMilliseconds = 0;

        // 36. Declare and initialize fields in the NumberTile class to keep track of the total milliseconds it takes the
            // tile to shrink (this should be a constant) and the elapsed shrink milliseconds so far (this should be a
            // variable) (Chapter 12, Week 6 for fields. Chapter 7, Week 3 for timers)
        // Shrinking support
        const int TOTAL_SHRINK_MILLISECONDS = 1000;
        int elapsedShrinkMilliseconds = 0;

        // 32. Add fields to the NumberTile class to tell if the tile is visible, 
        //        if the tile is blinking, and if the tile is
        //        shrinking. Initialize those fields appropriately (Chapter 12, Week 6)
        bool visible = true, blinking = false, shrinking = false;

 

        //33. Add clickStarted and buttonReleased fields to the NumberTile class. 
        bool clickStarted = false, buttonReleased = true ;

        // 66. Declare a field in the NumberTile class to hold a SoundBank object (Chapter 12, Week 6)
        SoundBank  soundbankObject;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="contentManager">the content manager</param>
        /// <param name="center">the center of the tile</param>
        /// <param name="sideLength">the side length for the tile</param>
        /// <param name="number">the number for the tile</param>
        /// <param name="correctNumber">the correct number</param>
        /// <param name="soundBank">the sound bank for playing cues</param>
        public NumberTile(ContentManager contentManager, Vector2 center, int sideLength,
            int number, int correctNumber, SoundBank soundBank)
        {
            // set original side length field
            this.originalSideLength = sideLength;

            // set sound bank field
            // 67. In the NumberTile constructor, add code to set the field from the previous step to the soundBank
                // parameter (Chapter 13, Week 6)
            soundbankObject = soundBank;

            // load content for the tile and create draw rectangle
            LoadContent(contentManager, number);
            drawRectangle = new Rectangle((int)center.X - sideLength / 2,
                 (int)center.Y - sideLength / 2, sideLength, sideLength);

            // set isCorrectNumber flag
            isCorrectNumber = number == correctNumber;
        }

        #endregion

        #region Public methods

        /// <summary>
        /// Updates the tile based on game time and mouse state
        /// </summary>
        /// <param name="gameTime">the current GameTime</param>
        /// <param name="mouse">the current mouse state</param>
        /// <return>true if the correct number was guessed, false otherwise</return>
        public bool Update(GameTime gameTime, MouseState mouse)
        {

            // 31. In the NumberTile Update method, add an if statement at the beginning of the method to set the source
            //        rectangle X property appropriately based on whether or not the mouse is over the draw rectangle for the tile.
            //        At this point, each of the tiles highlights when the mouse is over the tile and unhighlights when the mouse
            //        isn’t over the tile (Chapter 8, Week 4)
            //if (drawRectangle.Contains(mouse.X, mouse.Y))
            //    sourceRectangle.X = sourceRectangle.Width;
            //else
            //    sourceRectangle.X = 0;

                //    33. In the NumberTile
                //        Update method, change the if statement from Step 31 to detect mouse clicks on the number tile. You can
                //        use the MenuButton Update method code at the very end of Chapter 8 to help you with this (I also
                //        provided the code for that method in the zip file for this project). Replace the code (from Chapter 8) that
                //        changes the game state when a click is finished on the menu button with code that does the following
                //        instead: If the player just clicked on the tile and the tile corresponds to the correct number, set the tile is
                //        blinking field to true, otherwise set the tile is shrinking field to true. You need to detect clicks rather
                //        than left mouse button presses so the player can’t just “sweep” the mouse over the tiles to guess numbers
                //        (Chapter 12, Week 6 for fields. Chapter 8, Week 4 for using Chapter 8 code)

           

            //37. In the NumberTile Update method, add an if statement that checks if the tile is shrinking. If it is,
            //        update the elapsed shrink milliseconds field by adding
            //        game Time.ElapsedGameTime.Millisconds to it. Calculate the new side length for the tile using
            //        the original side length and the ratio between (total shrink milliseconds - elapsed shrink milliseconds) and
            //        total shrink milliseconds. CAUTION: Force the ratio to get calculated as a float, otherwise your tile
            //        will just disappear instead of gradually shrinking. Doing it this way makes the shrinking tile start at
            //        (almost) full size, then shrink down to 0. If the new side length is > 0, set the width and height of the draw
            //        rectangle to the new side length; otherwise, set the visible flag for the tile to false
            //        (Chapter 7, Week 3 for timers)

            //46. In the NumberTile Update method, change the original if clause 
            //    (that checked if the tile is shrinking)
            //    for the if statement from Step 37 to an else if instead. 
            //        Now add an if clause before that else if clause to
            //        check if the tile is blinking. You'll now have an if 
            //        statement with an if clause that checks if the tile is
            //        blinking, an else if clause that checks if the tile is 
            //        shrinking, and an else clause that highlights/unhighlights
            //        the tile and lets the player click the mouse button on the tile. 
            //        Basically, we’re doing different update
            //        processing on the tile based on the current tile state 
            //        (blinking, shrinking, or normal). (Chapter 7, Week 3)
            if(blinking)
            {
                //48. Inside the if clause you added in Step 46, add code to determine when 
                //    to make the tile invisible (after it’s
                //    done blinking). Note that I've already included elapsedBlinkMilliseconds and
                //    TOTAL_BLINK_MILLISECONDS fields for you to use for this. You’ll need to update the elapsed blink
                //    milliseconds field by adding gameTime.ElapsedGameTime.Milliseconds to it before deciding
                //    whether or not to stop the blinking animation (Chapter 7, Week 3 for timers)
                elapsedBlinkMilliseconds += gameTime.ElapsedGameTime.Milliseconds;
                if (elapsedBlinkMilliseconds >= TOTAL_BLINK_MILLISECONDS)
                {
                    blinking = false;
                    elapsedBlinkMilliseconds = 0;
                    //57. At this point, we never actually end up restarting a game. That's because the NumberTile Update
                    //    method always returns false. In the code in that method that sets visible for a blinking tile to false (see
                    //    step 48), change the code that sets visible to false to return true; instead. We don't want to return true
                    //    from this method right when the user picks the correct number because then they won't get to see any
                    //    blinking, so we return true when the blinking is done (Chapter 13, Week 6 for return)
                    return true ;
                }

                // 49. After the code you added in the previous step 
                    //(but still inside the if clause you added in Step 48), add code
                    //to update the animation frame as appropriate. I've already 
                    //included elapsedFrameMilliseconds and
                    //TOTAL_FRAME_MILLISECONDS fields for you to use for this. Add
                    //gameTime.ElapsedGameTime.Millisconds to the elapsed 
                    //frame milliseconds field, then use the
                    //new value to decide when to change to a new animation frame. 
                    //To change frames in the blinking
                    //animation, you need to move the source rectangle left or right 
                    //(depending on where the source rectangle
                    //currently is) after each frame ends to change the texels being displayed (Chapter 7, Week 3)
                    elapsedFrameMilliseconds += gameTime.ElapsedGameTime.Milliseconds;
                    if (elapsedFrameMilliseconds >= FRAME_BLINK_MILLISECONDS)
                    {
                        if (sourceRectangle.X == 0)
                            sourceRectangle.X = sourceRectangle.Width;
                        else
                            sourceRectangle.X = 0;
                        
                        elapsedFrameMilliseconds=0;
                    }
            }
            else if (shrinking)
            {
                elapsedShrinkMilliseconds += gameTime.ElapsedGameTime.Milliseconds;
                int newSideLength = (int)(originalSideLength *
                    ((TOTAL_SHRINK_MILLISECONDS - (float)elapsedShrinkMilliseconds) /
                    TOTAL_SHRINK_MILLISECONDS));

                if (newSideLength > 0)
                {
                    drawRectangle.Width = drawRectangle.Height = newSideLength;
                }
                else
                    visible = false;
            }
            // 38. At this point, clicking on any tile except the 8 tile should make the tile gradually shrink until it’s
            //    disappeared. Unfortunately, if you move the mouse off the shrinking tile it’s no longer highlighted, which
            //    doesn’t look right. In the NumberTile Update method, move the entire if statement you wrote in Step
            //    33 into an else clause for the if statement you wrote in Step 37 so the source rectangle is only changed if
            //    the tile isn’t shrinking. There are other ways to make sure the tile stays highlighted, but doing it this way is
            //    required for later steps (Chapter 7, Week 3)
            else
            {
                if (drawRectangle.Contains(mouse.X, mouse.Y))
                {

                    sourceRectangle.X = sourceRectangle.Width;
                    // check for click started on button
                    if (mouse.LeftButton == ButtonState.Pressed && buttonReleased)
                    {
                        clickStarted = true;
                        buttonReleased = false;
                    }
                    else if (mouse.LeftButton == ButtonState.Released)
                    {
                        buttonReleased = true;

                        // if click finished on button, change game state
                        if (clickStarted)
                        {
                            // Game1.ChangeState(clickState);
                            if (isCorrectNumber)
                            {
                                // 68. In the NumberTile Update method, add code to play the correctGuess cue when the player picks a tile
                                //corresponding to the correct number (right after you set the blinking flag to true) and to play the
                                //incorrectGuess cue when the player picks an incorrect tile (right after you set the shrinking flag to true)
                                //(Chapter 14, Week 7
                                soundbankObject.PlayCue("newGame");

                                blinking = true;
                                //45. In the NumberTile Update method, add two lines of code just after the line 
                                //  where you set the tile is
                                // blinking field to true. Your first new line of code should set the current 
                                // texture to the blinking texture.
                                // Your second new line of code should set the X property for your source 
                                // rectangle to 0 (Chapter 5, Week 2)
                                currentTexture = blinkingTileTexture2D;

                            }
                            else
                            {
                                shrinking = true;
                                soundbankObject.PlayCue("incorrectGuess");
                            }
                            clickStarted = false;
                        }



                    }
                }
                else
                {
                    sourceRectangle.X = 0;
                    // no clicking on this button
                    clickStarted = false;
                    buttonReleased = false;
                }
            }

            // if we get here, return false
            return false;
        }

        /// <summary>
        /// Draws the number tile
        /// </summary>
        /// <param name="spriteBatch">the SpriteBatch to use for the drawing</param>
        public void Draw(SpriteBatch spriteBatch)
        {
            // draw the tile
            // 25. Write the NumberTile Draw method to draw the tile texture using the draw ?????? and source rectangles
                // (Chapter 7, Week 3)
            // 35. In the NumberTile Draw method, only draw the tile if it’s visible (Chapter 7, Week 3)
            // 44. In the NumberTile Draw method, change the code to draw the current texture (Chapter 5, Week 2)
            if (visible)
                spriteBatch.Draw(currentTexture, drawRectangle, sourceRectangle, Color.White);
        }

        #endregion

        #region Private methods

        /// <summary>
        /// Loads the content for the tile
        /// </summary>
        /// <param name="contentManager">the content manager</param>
        /// <param name="number">the tile number</param>
        private void LoadContent(ContentManager contentManager, int number)
        {
            // convert the number to a string
            string numberString = ConvertIntToString(number);

            // load content for the tile and set source rectangle
                // 22. Write the NumberTile LoadContent method. I already provided code that uses the
                //  ConvertIntToString helper method to convert the integer to the string for the tile number (e.g.,
                //  converting 8 to "eight"). We use this string in the LoadContent method so we can load the correct
                //  graphics content. You also need to create a new source rectangle object in this method; make it “cover” the
                //  left half of the texture you just loaded (Chapter 7, Week 3)
            texture = contentManager.Load<Texture2D>(numberString);
//            texture = contentManager.Load<Texture2D>("eight");
//            sourceRectangle = new Rectangle(drawRectangle.X, drawRectangle.Y, (int)drawRectangle.Width / 2, drawRectangle.Height);
            sourceRectangle = new Rectangle(0,0, (int)texture.Width / 2, texture.Height);

            // 41. In the NumberTile LoadContent method, add code to load the blinking tile texture into the field from
                    // the previous step. Note that you can use the string for the 
                    // tile number to help build the name of the texture
                    // (all the blinking textures start with “blinking” followed 
                    // by the number string) (Chapter 6, Week 3)
            numberString = "blinking" + numberString;
            blinkingTileTexture2D = contentManager.Load<Texture2D>(numberString);

            // 43. In the NumberTile LoadContent method, add code to set the current texture to the non-blinking
                // texture (Chapter 3, Week 1)
            currentTexture = texture;

        }

        /// <summary>
        /// Converts an integer to a string for the corresponding number
        /// </summary>
        /// <param name="number">the integer to convert</param>
        /// <returns>the string for the corresponding number</returns>
        private String ConvertIntToString(int number)
        {
            switch (number)
            {
                case 1:
                    return "one";
                case 2:
                    return "two";
                case 3:
                    return "three";
                case 4:
                    return "four";
                case 5:
                    return "five";
                case 6:
                    return "six";
                case 7:
                    return "seven";
                case 8:
                    return "eight";
                case 9:
                    return "nine";
                default:
                    throw new Exception("Unsupported number for number tile");
            }

        }

        #endregion
    }
}
