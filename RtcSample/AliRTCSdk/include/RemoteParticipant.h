#if !defined(_REMOTE_PARTICIPANT_H_)
#define _REMOTE_PARTICIPANT_H_

#include <string>

class AliVideoCanvas;

class RemoteParticipant
{
public:
    RemoteParticipant();
    virtual ~RemoteParticipant();

    //
    // basic infomation
    //
    std::string userID;    // user ID returned from App Server
    std::string sessionID; // session ID returned from App Server. used to determine online/offline. "": offline
    std::string callID;    // used to determine publisher or not. "": not a publisher
    std::string displayName;

    std::string lastCallID; // remember last call id (possible callID erased but a late callback comes in)

    //
    // available tracks
    //
    std::string streamLabel;
    std::string audioTrackLabel; // not "" if has audio
    std::string cameraMasterTrackLabel;
    std::string cameraSlaveTrackLabel;
    std::string screenTrackLabel;
    bool hasCameraMaster; // whether has camera master content?
    bool hasCameraSlave;  // whether has camera slave content?
    bool hasScreenSharing; // whether has screen sharing content?

    //
    // stuff to subscribe
    //
    std::string videoToSubscribe[3]; // large capture video, small capture video, screen sharing video tracks. Non empty string stands for subscribe
    bool audioToSubscribe; // subscribe audio
	bool preferCameraMaster;

    //
    // subscribe result
    //
    std::string videoSubscribed[3];
    bool audioSubscribed;
    bool firstSubscribe;
    bool fristSubscribing;

    //
    // video sink
    //
    AliVideoCanvas *cameraCanvas; // video canvas for camera track (always not NULL)
    AliVideoCanvas *screenCanvas; // video canvas for screen track (always not NULL)

    //
    // audio sink
    //
    bool muteAudioPlaying; // mute speaker

    //
    // check basic info
    //
    bool isOnline();
    void setOffline();
    void clearPublisherInfo();

    //
    // check stuff to subscribe
    //
    // whether master video is going to request for camera track
    bool willSubscribeCameraTrackMaster();

    //
    // check subscribe result
    //
    bool isCameraVideoSubscribed();
    bool isScreenVideoSubscribed();

	bool isPublisher();
    
    void setDefaultSub(bool autoSub);
};

#endif /* _REMOTE_PARTICIPANT_H_ */
