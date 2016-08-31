using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Configuration;
using System.Diagnostics;
using System.Threading;
using static ChappieMonogame.WebRequestManager;

namespace ChappieMonogame
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class Game1 : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        private ControllerState _state { get; set; }
        private int _controller { get; set; }
        private WebRequestManager _requestManager { get; set; }

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            _state = new ControllerState();
            _controller = Convert.ToInt32(ConfigurationManager.AppSettings["Controller"]);
            _requestManager = new WebRequestManager();
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

            // TODO: use this.Content to load your game content here
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// game-specific content.
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
            var isConnected = GamePad.GetState(_controller).IsConnected;

            if (isConnected)
            {
                var buttons = GamePad.GetState(_controller).Buttons;
                var triggers = GamePad.GetState(_controller).Triggers;
                var thumbSticks = GamePad.GetState(_controller).ThumbSticks;

                if (buttons.Start != _state.Start)
                {
                    if (buttons.Start == ButtonState.Pressed)
                        _requestManager.SendRequest(command.stop);


                    Debug.Print("Start: " + buttons.Start.ToString());
                    _state.Start = buttons.Start;
                }

                if (buttons.A != _state.A)
                {
                    if (buttons.A == ButtonState.Pressed)
                        _requestManager.SendRequest(command.backward);
                    //else
                    //    _requestManager.SendRequest(command.stop);

                    Debug.Print("A: " + buttons.A.ToString() + " - " + _state.A);
                    _state.A = buttons.A;
                }

                if (buttons.B != _state.B)
                {
                    if (buttons.B == ButtonState.Pressed)
                        _requestManager.SendRequest(command.right);
                    //else
                    //    _requestManager.SendRequest(command.stop);

                    Debug.Print("B: " + buttons.B.ToString() + " - " + _state.B);
                    _state.B = buttons.B;
                }

                if (buttons.X != _state.X)
                {
                    if (buttons.X == ButtonState.Pressed)
                        _requestManager.SendRequest(command.left);
                    //else
                    //    _requestManager.SendRequest(command.stop);

                    Debug.Print("X: " + buttons.X.ToString() + " - " + _state.X);
                    _state.X = buttons.X;
                }

                if (buttons.Y != _state.Y)
                {
                    if (buttons.Y == ButtonState.Pressed)
                        _requestManager.SendRequest(command.forward);
                    //else
                    //    _requestManager.SendRequest(command.stop);

                    Debug.Print("Y: " + buttons.Y.ToString() + " - " + _state.Y);
                    _state.Y = buttons.Y;
                }

                if (triggers.Right != _state.right)
                {
                    _requestManager.SendRequest(command.speed, (Math.Round(255 * triggers.Right)).ToString());
                    Debug.Print("Right: " + (Math.Round(255 * triggers.Right)));
                    _state.right = triggers.Right;
                }

                if (thumbSticks.Left.X != _state.leftX)
                {
                    _requestManager.SendRequest(command.curve, (Math.Round(20 * -(thumbSticks.Left.X))).ToString());
                    Debug.Print("Left Thumb: " + (Math.Round(20 * -(thumbSticks.Left.X))).ToString());
                    _state.leftX = thumbSticks.Left.X;
                }

                Thread.Sleep(200);
            }
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // TODO: Add your drawing code here

            base.Draw(gameTime);
        }
    }
}
