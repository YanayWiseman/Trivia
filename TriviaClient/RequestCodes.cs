using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    enum RequestCode
    {
        LoginRequestCode = 100,
        SignupRequestCode,
        LogoutRequestCode,
        GetRoomsRequestCode,
        GetPlayersInRoomRequestCode,
        HighScoreRequestCode,
        GetPersonalStatsRequestCode,
        JoinRoomRequestCode,
        CreateRoomRequestCode,
        CloseRoomRequestCode,
        StartGameRequestCode,
        GetRoomStateRequestCode,
        LeaveRoomRequestCode,
        GetQuestionRequestCode,
        SubmitAnswerRequestCode,
        GetGameResultsRequestCode,
        LeaveGameRequestCode
    }
}
