#pragma once

#define SIGNIN_REQUEST 200
#define SIGNIN_REPLY 102

#define SIGNOUT_REQUEST 201

#define SIGNUP_REQUEST 203
#define SIGNUP_REPLY 104

#define EXISTING_ROOM_REQUEST 205
#define EXISTING_ROOM_REPLY 106

#define USERS_IN_ROOM_REQUEST 207
#define USERS_IN_ROOM_REPLY 108

#define JOIN_ROOM_REQUEST 209
#define JOIN_ROOM_REPLY 110

#define LEAVE_ROOM_REQUEST 211
#define LEAVE_ROOM_REPLY_SUCCESS 1120

#define CREATE_ROOM_REQUEST 213
#define CREATE_ROOM_REPLY 114
//NEED TO MODIFY - AND MODIFY USER.CPP
#define CREATE_ROOM_FAILED 1141
#define CREATE_ROOM_SUCSESS 1140

#define CLOSE_ROOM_REQUEST 215
#define CLOSE_ROOM_REPLY 116

#define START_GAME_REQUEST 217
#define SEND_QUESTION 118

#define CLIENT_ANSWER 219
#define CLIENT_ANSWER_REPLY 120

#define END_GAME 121

#define LEAVE_GAME_REQUEST 222

#define BEST_SCORE_REQUEST 223
#define BEST_SCORE_REPLY 124

#define PERSONAL_STATE_REQUEST 225
#define PERSONAL_STATE_REPLY 126

#define EXIT 299
