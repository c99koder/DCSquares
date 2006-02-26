/*
 *  NetworkLobby.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 2/19/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>

class NetworkLobby : public GenericMenu {
public:
	NetworkLobby(char *host, char *user, char *pass);
	void FadeIn();
	void FadeOut();
	void controlPerFrame();
	
	void insertText(int channel, char *text);
	void reloadUsers();
	
protected:
	void inputEvent(const Event & evt);
	
private:
	RefPtr<Banner> bg;
	RefPtr<Box> box;
	RefPtr<Layer> users,chat;
	RefPtr<Label> text;
	std::list< RefPtr<Label> > chatText;
	int chatOffset;
};
