#include "NamespaceProxy.hpp"

class NamespaceProxy::impl {
	friend class NamespaceProxy;
	std::string getNamespace() {
		return namespace_;
	}

	void setNamespace(const std::string& name) {
		namespace_ = name;
	}

	std::string namespace_;
};

NamespaceProxy::NamespaceProxy() : pimpl_(new impl()) {}

std::string NamespaceProxy::getNamespace() {
	return pimpl_->getNamespace();
}

void NamespaceProxy::setNamespace(const std::string& name) {
	return pimpl_->setNamespace(name);
}

NamespaceProxy::~NamespaceProxy()
{}
