#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#include <GosuImpl/MacUtility.hpp>
#include <boost/scoped_ptr.hpp>
#include <algorithm>

namespace Gosu
{
    class ALChannelManagement : boost::noncopyable
    {
        static ALCdevice* alDevice;
        static ALCcontext* alContext;

        enum { NUM_SOURCES = 32 }; // This is what the iPhone supports, I hear.
        NSUInteger alSources[NUM_SOURCES];
        NSUInteger currentToken;
        NSUInteger currentTokens[NUM_SOURCES];
        
    public:
        enum { NO_TOKEN = -1, NO_SOURCE = -1, NO_FREE_CHANNEL = -1 };
    
        ALChannelManagement()
        {
            // Open preferred device
            alDevice = alcOpenDevice(0);
            alContext = alcCreateContext(alDevice, 0);
            alcMakeContextCurrent(alContext);
            alGenSources(NUM_SOURCES, alSources);
            currentToken = 0;
            std::fill(currentTokens, currentTokens + NUM_SOURCES,
                static_cast<NSUInteger>(NO_TOKEN));
        }
        
        ~ALChannelManagement()
        {
            alDeleteSources(NUM_SOURCES, alSources);
            alcMakeContextCurrent(0);
            alcDestroyContext(alContext);
            alcCloseDevice(alDevice);
        }
        
        std::pair<int, int> reserveChannel()
        {
            int i;
            for (i = 0; i <= NUM_SOURCES; ++i)
            {
                if (i == NUM_SOURCES)
                    return std::make_pair<int, int>(NO_FREE_CHANNEL, NO_TOKEN);
                if (currentTokens[i] == NO_TOKEN)
                    break;
                ALint state;
                alGetSourcei(alSources[i], AL_SOURCE_STATE, &state);
                if (state != AL_PLAYING && state != AL_PAUSED)
                    break;
            }
            ++currentToken;
            currentTokens[i] = currentToken;
            return std::make_pair<int, int>(i, currentToken);
        }
        
        int sourceIfStillPlaying(int channel, int token) const
        {
            if (currentTokens[channel] == token)
                return alSources[channel];
            return NO_SOURCE;
        }
    };
    ALCdevice* ALChannelManagement::alDevice = 0;
    ALCcontext* ALChannelManagement::alContext = 0;
    
    boost::scoped_ptr<ALChannelManagement> alChannelManagement;
}