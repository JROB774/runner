#ifndef INCLUDE_J_TIME
#define INCLUDE_J_TIME

class J_Timer
{
	public:

		J_Timer() = default;
		J_Timer(const J_Timer&) = default;
        J_Timer(J_Timer&&) = default;

		J_Timer& operator=(const J_Timer&) = default;
		J_Timer& operator=(J_Timer&&) = default;

		void start();
		void stop();

		void pause();

		Uint32 getTicks() const;

		~J_Timer() = default;

	private:

		bool started, paused;
		Uint32 startTicks, pauseTicks;
};

inline bool operator==(const J_Timer&, const J_Timer&);
inline bool operator!=(const J_Timer&, const J_Timer&);
inline bool operator<(const J_Timer&, const J_Timer&);
inline bool operator>(const J_Timer&, const J_Timer&);
inline bool operator<=(const J_Timer&, const J_Timer&);
inline bool operator>=(const J_Timer&, const J_Timer&);



class J_Time
{
    public:

        static const int SECOND;

        static void setDelta(const double);

        static double getDelta();
        static Uint32 getTime();

    private:

        static double delta;
};

#endif // INCLUDE_J_TIME
