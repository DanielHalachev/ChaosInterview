#pragma once
class ProgressNotifier {
   public:
	ProgressNotifier() = default;
	ProgressNotifier(const ProgressNotifier& other) = delete;
	ProgressNotifier& operator=(const ProgressNotifier& other) = delete;
	ProgressNotifier(ProgressNotifier&& other) = default;
	ProgressNotifier& operator=(ProgressNotifier&& other) = default;
	virtual void reportProgress(unsigned short done, unsigned short total) = 0;
	virtual ~ProgressNotifier() = default;
};