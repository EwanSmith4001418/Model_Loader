#pragma once

//! Event Class.
/*
	An abstract base class for concrete event classes to inherit from
*/

class Event
{
public:
	//! Default constructor
	/* !COnstructs a base event class object. Sets Handled to false.
	*/
	Event() : m_bHandled(false) {}
	//! Destructor
	virtual ~Event() {};
	//! using the 'using' command to create an alias for const char*
	using DescriptorType = const char*;
	//! Returns the descriptor type of the event
	/*! ABstract function to be implemented in derived classes
	*	@return DescriptorType returns the type of Event as a const char*
	*/
	virtual DescriptorType type() const = 0;
	/*!
	* Function used to set if event has been handled. Events that are handled do not report
	* to any subsequent observers
	*/
	void Handled() { m_bHandled = true; }
	/*! Function to set that an event has been handled
	*/
	bool IsHandled() { return m_bHandled; }
private:
	//! variable indicates if event has been processed or not
	bool m_bHandled;
};