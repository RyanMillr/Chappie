using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace ChappieMonogame
{
    /// <summary>
    ///     Sends commands to Chappie via http requests.
    /// </summary>
    class WebRequestManager
    {
        public enum command
        {
            forward,
            left,
            right,
            backward,
            stop,
            speed,
            curve
        }

        private string _baseURL { get; set; }

        public WebRequestManager()
        {
            _baseURL = ConfigurationManager.AppSettings["BaseUrl"];
        }

        /// <summary>
        ///     Send synchronous requests to Chappie. Didn't go with asynch
        ///     to prevent from swamping the web server. This means that the 
        ///     next state change won't be picked up until after the current 
        ///     web request has finished processing.
        /// </summary>
        /// <param name="command"></param>
        /// <param name="param"></param>
        public void SendRequest(command command, string param = null)
        {
            try
            {
                string url;
                if (param == null)
                    url = _baseURL + "/" + command;
                else
                    url = _baseURL + "/" + command + "?params=" + param;

                var request = WebRequest.Create(url);
                request.Timeout = 500;

                Console.WriteLine("Sending request to:" + url);
                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    Console.WriteLine(response.StatusCode);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error sending web request: " + ex.Message);
            }
        }
    }
}
