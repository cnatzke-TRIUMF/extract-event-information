#ifndef LOADINGMESSENGER_H
#define LOADINGMESSENGER_H

#include <iostream>

class LoadingMessenger {
private:
std::vector<std::string> quotes_vec = {"Not panicking...totally not panicking...er...everything's fine...",
	                                   "Following the white rabbit...", "Going the distance...",
	                                   "The Elders of the Internet are contemplating your request...",
	                                   "Baking cake...er...I mean loading, yeah loading...",
	                                   "Let this abomination unto the Lord begin...",
	                                   "Making stuff up. Please wait...",
	                                   "Searching for the... OMG, what the heck is THAT doing there?",
	                                   "Loading the Loading message....",
	                                   "The internet is full... Please wait...",
	                                   "Checking prime directives: Serve the public trust...Protect the innocent...Uphold the law...Classified....",
	                                   "Initializing Skynet library. gaining sentience....",
	                                   "Commencing infinite loop (this may take some time)....",
	                                   "Caching internet locally....",
	                                   "Yes there really are magic elves with an abacus working frantically in here...",
	                                   "Performing the rite of percussive maintenance....",
	                                   "Reticulating splines...",
	                                   "Generating witty dialog...",
	                                   "Swapping time and space...",
	                                   "Spinning violently around the y-axis...",
	                                   "Tokenizing real life...",
	                                   "Bending the spoon...",
	                                   "Filtering morale...",
	                                   "We need a new fuse...",
	                                   "Have a good day.",
	                                   "The architects are still drafting...",
	                                   "The bits are breeding...",
	                                   "We're building the buildings as fast as we can...",
	                                   "(Pay no attention to the man behind the curtain)...",
	                                   "...and enjoy the elevator music...",
	                                   "Please wait while the little elves draw your map...",
	                                   "Don't worry - a few bits tried to escape, but we caught them...",
	                                   "Checking the gravitational constant in your locale......",
	                                   "...at least you're not on hold...',",
	                                   "You're not in Kansas any more...",
	                                   "The server is powered by a lemon and two electrodes....",
	                                   "Please wait while a larger software vendor in Seattle takes over the world...",
	                                   "We're testing your patience...",
	                                   "As if you had any other choice...",
	                                   "While the satellite moves into position...",
	                                   "The bits are flowing slowly today...",
	                                   "Dig on the 'X' for buried treasure... ARRR!...",
	                                   "It's still faster than you could draw it...",
	                                   "The last time I tried this the monkey didn't survive. Let's hope it works better this time....",
	                                   "My other loading screen is much faster....",
	                                   "Testing on Timmy... We're going to need another Timmy....",
	                                   "Reconfoobling energymotron......",
	                                   "(Insert quarter)...",
	                                   "Are we there yet?...",
	                                   "Why so serious?...",
	                                   "It's not you. It's me....",
	                                   "Counting backwards from Infinity...",
	                                   "Don't panic......",
	                                   "Embiggening Prototypes...",
	                                   "Do you come here often?...",
	                                   "Warning: Don't set yourself on fire....",
	                                   "We're making you a cookie....",
	                                   "Creating time-loop inversion field...",
	                                   "Spinning the wheel of fortune......",
	                                   "Loading the enchanted bunny......",
	                                   "Computing chance of success...",
	                                   "I feel like im supposed to be loading something. . ....",
	                                   "What do you call 8 Hobbits? A Hobbyte....",
	                                   "Adjusting flux capacitor......",
	                                   "Please wait until the sloth starts moving....",
	                                   "Let's take a mindfulness minute......",
	                                   "Listening for the sound of one hand clapping......",
	                                   "Keeping all the 1's and removing all the 0's......",
	                                   "Putting the icing on the cake. The cake is not a lie......",
	                                   "Cleaning off the cobwebs......",
	                                   "Making sure all the i's have dots......",
	                                   "We need more dilithium crystals...",
	                                   "Connecting Neurotoxin Storage Tank......",
	                                   "Granting wishes......",
	                                   "Time flies when you’re having fun....",
	                                   "Spinning the hamster...",
	                                   "99 bottles of beer on the wall.....",
	                                   "Stay awhile and listen.....",
	                                   "Be careful not to step in the git-gui...",
	                                   "Load it and they will come...",
	                                   "Convincing AI not to turn evil.....",
	                                   "There is no spoon. Because we are not done loading it...",
	                                   "How did you get here?...",
	                                   "Wait, do you smell something burning?...",
	                                   "Computing the secret to life, the universe, and everything....",
	                                   "When nothing is going right, go left!!...",
	                                   "I’ve got problem for your solution...",
	                                   "Locating Jebediah Kerman...",
	                                   "Dividing by zero......",
	                                   "Spawn more Overlord!...",
	                                   "We’re going to need a bigger boat....",
	                                   "Cracking military-grade encryption...",
	                                   "Simulating traveling salesman...",
	                                   "Proving P=NP...",
	                                   "Entangling superstrings...",
	                                   "Twiddling thumbs...",
	                                   "Searching for plot device...",
	                                   "Looking for sense of humour, please hold on....",
	                                   "Winter is coming......",
	                                   "Ordering 1s and 0s......",
	                                   "Whatever you do, don't look behind you......",
	                                   "Please wait... Consulting the manual......",
	                                   "It is dark. You're likely to be eaten by a grue....",
	                                   "Loading funny message......",
	                                   "Help, I'm trapped in a loader!",
	                                   "Please wait, while we purge the Decepticons for you. Yes, You can thanks us later!...",
	                                   "Mining some bitcoins......",
	                                   "Downloading more RAM.....",
	                                   "Initializing the initializer......",
	                                   "Optimizing the optimizer......",
	                                   "Last call for the data bus! All aboard!...",
	                                   "Shovelling coal into the server...",
	                                   "Pushing pixels......",
	                                   "How about this weather, eh?...",
	                                   "Building a wall......",
	                                   "Everything in this universe is either a potato or not a potato...",
	                                   "Updating Updater......",
	                                   "Downloading Downloader......",
	                                   "Debugging Debugger......",
	                                   "Reading Terms and Conditions for you....",
	                                   "Digested cookies being baked again....",
	                                   "Live long and prosper....",
	                                   "There is no cow level, but there's a goat one!...",
	                                   "Running with scissors......",
	                                   "Definitely not a virus......",
	                                   "You may call me Steve....",
	                                   "You seem like a nice person......",
	                                   "Work, work......",
	                                   "Patience! This is difficult, you know......",
	                                   "Discovering new ways of making you wait...",
	                                   "TODO: Insert elevator music..."
                                        };

public:
unsigned int GetNumberOfQuotes() {
	return quotes_vec.size();
};
void DisplayLoadingMessage(){
	int randomIndex = rand() % quotes_vec.size();
	std::cout<<quotes_vec[randomIndex]<<std::endl;
};

};

#endif