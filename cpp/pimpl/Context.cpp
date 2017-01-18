#include "Context.hpp"

class Context::impl {
	friend class Context;
	std::string getNamespace() {
		return netns;
	}

	void setNamespace(const std::string& name) {
		netns = name;
	}

	std::string netns;
};

Context::Context() : pimpl_(new impl()) {}

std::string Context::getNamespace() {
	return pimpl_->getNamespace();
}

void Context::setNamespace(const std::string& name) {
	return pimpl_->setNamespace(name);
}

Context::~Context()
{}
