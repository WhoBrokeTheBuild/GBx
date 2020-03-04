#ifndef GBX_H
#define GBX_H

extern int VerboseLevel;

void reset();

void run();

void stop();

void load(const char * filename);

void save(const char * filename);

#endif // GBX_H