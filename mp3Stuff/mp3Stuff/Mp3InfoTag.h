#pragma once

#include <cstdint>
#include "hxlstr.h"
#include "Mp3InfoFrm.h"


class Mp3InfoTag
{
private:
	Mp3InfoFrm** m_pframes;
	int m_pos;


public:
	class iterator {
	private:
		int m_pos;
		Mp3InfoTag& m_theTag;

	public:
		iterator(int pos, Mp3InfoTag& aTag) : m_pos(pos), m_theTag(aTag) {	}

		iterator& operator++(int) {
			m_pos++;
			return *this;
		}

		iterator& operator++() {
			m_pos++;
			return *this;
		}

		Mp3InfoFrm& operator*() {
			return m_theTag.get(m_pos);
		}

		bool operator!=(const iterator& other) const {
			return m_pos != other.m_pos;
		}
	};

	iterator begin() {
		return iterator(0, *this);
	}

	iterator end() {
		return iterator(m_pos, *this);
	}


	void add(Mp3InfoFrm& aFrame) {

		m_pframes[m_pos] = new Mp3InfoFrm;
		*m_pframes[m_pos] = aFrame;

		m_pos++;
	}

	Mp3InfoFrm& get(int i) {
		return *((Mp3InfoFrm*)m_pframes[i]);
	}

	int size() {
		return m_pos;
	}

	Mp3InfoTag() : m_pos(0) {
		m_pframes = new Mp3InfoFrm* [100];	//up to 100 frame pointers
	};

	
	~Mp3InfoTag() {
		for (int i = 0; i < m_pos; ++i) {
			delete[] m_pframes[i];
		}
	}
};



