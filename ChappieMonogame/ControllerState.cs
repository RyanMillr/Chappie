using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChappieMonogame
{
    class ControllerState
    {
        public ButtonState A { get; set; } = ButtonState.Released;
        public ButtonState B { get; set; } = ButtonState.Released;
        public ButtonState X { get; set; } = ButtonState.Released;
        public ButtonState Y { get; set; } = ButtonState.Released;
        public ButtonState Start { get; set; } = ButtonState.Released;
        public float left { get; set; } = 0;
        public float right { get; set; } = 0;
        public float leftX { get; set; } = 0;
        public float leftY { get; set; } = 0;
        public float rightX { get; set; } = 0;
        public float rightY { get; set; } = 0;
    }
}
