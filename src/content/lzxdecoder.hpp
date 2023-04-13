#ifndef XNA_CONTENT_LZXDECODER_HPP
#define XNA_CONTENT_LZXDECODER_HPP

#include "../csharp/integralnumeric.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include "../csharp/stream/stream.hpp"

//LzxConstants
namespace xna {
	struct LzxConstants {
		static constexpr csushort MIN_MATCH = 2;
		static constexpr csushort MAX_MATCH = 257;
		static constexpr csushort NUM_CHARS = 256;

		enum class BLOCKTYPE : csint {
			INVALID = 0,
			VERBATIM = 1,
			ALIGNED = 2,
			UNCOMPRESSED = 3
		};

		static constexpr csushort PRETREE_NUM_ELEMENTS = 20;
		static constexpr csushort ALIGNED_NUM_ELEMENTS = 8;
		static constexpr csushort NUM_PRIMARY_LENGTHS = 7;
		static constexpr csushort NUM_SECONDARY_LENGTHS = 249;

		static constexpr csushort PRETREE_MAXSYMBOLS = PRETREE_NUM_ELEMENTS;
		static constexpr csushort PRETREE_TABLEBITS = 6;
		static constexpr csushort MAINTREE_MAXSYMBOLS = NUM_CHARS + 50 * 8;
		static constexpr csushort MAINTREE_TABLEBITS = 12;
		static constexpr csushort LENGTH_MAXSYMBOLS = NUM_SECONDARY_LENGTHS + 1;
		static constexpr csushort LENGTH_TABLEBITS = 12;
		static constexpr csushort ALIGNED_MAXSYMBOLS = ALIGNED_NUM_ELEMENTS;
		static constexpr csushort ALIGNED_TABLEBITS = 7;

		static constexpr csushort LENTABLE_SAFETY = 64;
	};
}

//LzxState
namespace xna {
	struct LzxState {
		csuint R0{ 1 };
		csuint R1{ 1 };
		csuint R2{ 1 };
		csushort main_elements{ 0 };
		csint header_read{ 0 };
		LzxConstants::BLOCKTYPE	block_type{ LzxConstants::BLOCKTYPE::INVALID };
		csuint block_length{ 0 };
		csuint block_remaining{ 0 };
		csuint frames_read{ 0 };
		csint intel_filesize{ 0 };
		csint intel_curpos{ 0 };
		csint intel_started{ 0 };

		std::vector<csushort> PRETREE_table;
		std::vector<csbyte> PRETREE_len;
		std::vector<csushort> MAINTREE_table;
		std::vector<csbyte> MAINTREE_len;
		std::vector<csushort> LENGTH_table;
		std::vector<csbyte> LENGTH_len;
		std::vector<csushort> ALIGNED_table;
		std::vector<csbyte> ALIGNED_len;

		csuint actual_size{ 0 };
		std::vector<csbyte>	window;
		csuint window_size{ 0 };
		csuint window_posn{ 0 };
	};
}

//BitBuffer
namespace xna {
	struct BitBuffer {
		BitBuffer(std::shared_ptr<cs::Stream>& stream) :
			byteStream(stream) {
		}

		constexpr void InitBitStream() {
			buffer = 0;
			bitsleft = 0;
		}

		void EnsureBits(csbyte bits) {
			while (bitsleft < bits) {
				const csint lo = tobyte(byteStream->ReadByte());
				const csint hi = tobyte(byteStream->ReadByte());

				buffer |= touint(((hi << 8) | lo) << (sizeof(csuint) * 8 - 16 - bitsleft));
				bitsleft += 16;
			}
		}

		constexpr csuint PeekBits(csbyte bits) {
			return (buffer >> ((sizeof(csuint) * 8) - bits));
		}

		constexpr void RemoveBits(csbyte bits) {
			buffer <<= bits;
			bitsleft -= bits;
		}

		csuint ReadBits(csbyte bits) {
			csuint ret = 0;

			if (bits > 0) {
				EnsureBits(bits);
				ret = PeekBits(bits);
				RemoveBits(bits);
			}

			return ret;
		}

		constexpr csuint GetBuffer() {
			return buffer;
		}

		constexpr csbyte GetBitsLeft() {
			return bitsleft;
		}

	private:
		csuint buffer{ 0 };
		csbyte bitsleft{ 0 };
		std::shared_ptr<cs::Stream> byteStream;

	};
}

//LzxDecoder
namespace xna {
	struct LzxDecoder {
		LzxDecoder(csint window) {
			if (window < 15)
				window = 15;

			if (window > 21)
				window = 21;

			auto wndsize = touint(1 << window);

			m_state.window.resize(wndsize, 0xDC);
			m_state.actual_size = wndsize;
			m_state.window_size = wndsize;

			fillExtraBits();
			fillPositionBase();

			csint posn_slots;

			if (window == 20)
				posn_slots = 42;
			else if (window == 21)
				posn_slots = 50;
			else posn_slots = window << 1;

			m_state.main_elements = toushort(LzxConstants::NUM_CHARS + (posn_slots << 3));

			m_state.PRETREE_table.resize((1 << LzxConstants::PRETREE_TABLEBITS) + (LzxConstants::PRETREE_MAXSYMBOLS << 1));
			m_state.PRETREE_len.resize(LzxConstants::PRETREE_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);
			m_state.MAINTREE_table.resize((1 << LzxConstants::MAINTREE_TABLEBITS) + (LzxConstants::MAINTREE_MAXSYMBOLS << 1));
			m_state.MAINTREE_len.resize(LzxConstants::MAINTREE_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);
			m_state.LENGTH_table.resize((1 << LzxConstants::LENGTH_TABLEBITS) + (LzxConstants::LENGTH_MAXSYMBOLS << 1));
			m_state.LENGTH_len.resize(LzxConstants::LENGTH_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);
			m_state.ALIGNED_table.resize((1 << LzxConstants::ALIGNED_TABLEBITS) + (LzxConstants::ALIGNED_MAXSYMBOLS << 1));
			m_state.ALIGNED_len.resize(LzxConstants::ALIGNED_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);

			for (size_t i = 0; i < LzxConstants::MAINTREE_MAXSYMBOLS; ++i)
				m_state.MAINTREE_len[i] = 0;

			for (size_t i = 0; i < LzxConstants::LENGTH_MAXSYMBOLS; ++i)
				m_state.LENGTH_len[i] = 0;
		}

		static std::vector<csuint> position_base;
		static std::vector<csbyte> extra_bits;

		csint Decompress(std::shared_ptr<cs::Stream>& inData, csint inLen, cs::Stream outData, csint outLen) {
			BitBuffer bitbuf(inData);

			cslong startpos = inData->Position();
			cslong endpos = inData->Position() + inLen;

			auto& window = m_state.window;

			auto window_posn = m_state.window_posn;
			auto window_size = m_state.window_size;
			auto R0 = m_state.R0;
			auto R1 = m_state.R1;
			auto R2 = m_state.R2;
			csuint i, j = 0;
			auto togo = outLen;

			csint this_run, main_element, match_length, match_offset, length_footer, extra, verbatim_bits = 0;
			csint rundest, runsrc, copy_length, aligned_bits = 0;

			bitbuf.InitBitStream();

			if (m_state.header_read == 0) {
				auto intel = bitbuf.ReadBits(1);

				if (intel != 0) {
					i = bitbuf.ReadBits(16);
					j = bitbuf.ReadBits(16);
					m_state.intel_filesize = toint((i << 16) | j);
				}

				m_state.header_read = 1;
			}

			while (togo > 0) {
				if (m_state.block_remaining == 0) {

					if (m_state.block_type == LzxConstants::BLOCKTYPE::UNCOMPRESSED) {
						if ((m_state.block_length & 1) == 1) inData->ReadByte();
						bitbuf.InitBitStream();
					}

					m_state.block_type = static_cast<LzxConstants::BLOCKTYPE>(bitbuf.ReadBits(3));
					i = bitbuf.ReadBits(16);
					j = bitbuf.ReadBits(8);
					m_state.block_remaining = m_state.block_length = touint((i << 8) | j);

					switch (m_state.block_type)
					{
					case LzxConstants::BLOCKTYPE::ALIGNED: {
						for (i = 0, j = 0; i < 8; i++) {
							j = bitbuf.ReadBits(3); m_state.ALIGNED_len[i] = tobyte(j);
						}

						MakeDecodeTable(LzxConstants::ALIGNED_MAXSYMBOLS, LzxConstants::ALIGNED_TABLEBITS,
							m_state.ALIGNED_len, m_state.ALIGNED_table);

						ReadLengths(m_state.MAINTREE_len, 0, 256, bitbuf);
						ReadLengths(m_state.MAINTREE_len, 256, m_state.main_elements, bitbuf);

						MakeDecodeTable(LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS,
							m_state.MAINTREE_len, m_state.MAINTREE_table);

						if (m_state.MAINTREE_len[0xE8] != 0)
							m_state.intel_started = 1;

						ReadLengths(m_state.LENGTH_len, 0, LzxConstants::NUM_SECONDARY_LENGTHS, bitbuf);
						MakeDecodeTable(LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS,	m_state.LENGTH_len, m_state.LENGTH_table);
						break;
					}
					case LzxConstants::BLOCKTYPE::VERBATIM: {
						ReadLengths(m_state.MAINTREE_len, 0, 256, bitbuf);
						ReadLengths(m_state.MAINTREE_len, 256, m_state.main_elements, bitbuf);
						MakeDecodeTable(LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS,	m_state.MAINTREE_len, m_state.MAINTREE_table);

						if (m_state.MAINTREE_len[0xE8] != 0) 
							m_state.intel_started = 1;

						ReadLengths(m_state.LENGTH_len, 0, LzxConstants::NUM_SECONDARY_LENGTHS, bitbuf);
						MakeDecodeTable(LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS,
							m_state.LENGTH_len, m_state.LENGTH_table);
						break;
					}
					case LzxConstants::BLOCKTYPE::UNCOMPRESSED: {
						m_state.intel_started = 1;
						bitbuf.EnsureBits(16);
						
						if (bitbuf.GetBitsLeft() > 16) 
							inData->Seek(-2, cs::SeekOrigin::Current);

						csbyte hi, mh, ml, lo;

						lo = tobyte(inData->ReadByte());
						ml = tobyte(inData->ReadByte());
						mh = tobyte(inData->ReadByte());
						hi = tobyte(inData->ReadByte());
						R0 = touint(lo | ml << 8 | mh << 16 | hi << 24);
						lo = tobyte(inData->ReadByte());
						ml = tobyte(inData->ReadByte());
						mh = tobyte(inData->ReadByte());
						hi = tobyte(inData->ReadByte());
						R1 = touint(lo | ml << 8 | mh << 16 | hi << 24);
						lo = tobyte(inData->ReadByte());
						ml = tobyte(inData->ReadByte());
						mh = tobyte(inData->ReadByte());
						hi = tobyte(inData->ReadByte());
						R2 = touint(lo | ml << 8 | mh << 16 | hi << 24);
						break; 
					}

					default:
						return -1;
					}
				}

				
				if (inData->Position() > (startpos + inLen)) {
					if (inData->Position() > (startpos + inLen + 2) || bitbuf.GetBitsLeft() < 16)
						return -1;
				}

				while ((this_run = toint(m_state.block_remaining)) > 0 && togo > 0) {
					if (this_run > togo) 
						this_run = togo;

					togo -= this_run;
					m_state.block_remaining -= touint(this_run);
					
					window_posn &= window_size - 1;
					
					if ((window_posn + this_run) > window_size)
						return -1;

					switch (m_state.block_type) {
					case LzxConstants::BLOCKTYPE::VERBATIM: {
						while (this_run > 0) {
							main_element = toint(ReadHuffSym(m_state.MAINTREE_table, m_state.MAINTREE_len, LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS, bitbuf));
							if (main_element < LzxConstants::NUM_CHARS) {
								window[window_posn++] = tobyte(main_element);
								this_run--;
							}
							else {
								main_element -= LzxConstants::NUM_CHARS;
								match_length = main_element & LzxConstants::NUM_PRIMARY_LENGTHS;

								if (match_length == LzxConstants::NUM_PRIMARY_LENGTHS) {
									length_footer = toint(ReadHuffSym(m_state.LENGTH_table, m_state.LENGTH_len, LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS, bitbuf));
									match_length += length_footer;
								}

								match_length += LzxConstants::MIN_MATCH;
								match_offset = main_element >> 3;

								if (match_offset > 2) {
									if (match_offset != 3) {
										extra = extra_bits[match_offset];
										verbatim_bits = toint(bitbuf.ReadBits(tobyte(extra)));
										match_offset = toint(position_base[match_offset]) - 2 + verbatim_bits;
									}
									else {
										match_offset = 1;
									}

									R2 = R1;
									R1 = R0;
									R0 = touint(match_offset);
								}
								else if (match_offset == 0) {
									match_offset = toint(R0);
								}
								else if (match_offset == 1) {
									match_offset = toint(R1);
									R1 = R0;
									R0 = touint(match_offset);
								}
								else {
									match_offset = toint(R2);
									R2 = R0;
									R0 = touint(match_offset);
								}

								rundest = toint(window_posn);
								this_run -= match_length;

								if (window_posn >= match_offset) {
									runsrc = rundest - match_offset;
								}
								else {
									runsrc = rundest + (toint(window_size) - match_offset);
									copy_length = match_offset - toint(window_posn);

									if (copy_length < match_length) {
										match_length -= copy_length;
										window_posn += touint(copy_length);

										while (copy_length-- > 0)
											window[rundest++] = window[runsrc++];

										runsrc = 0;
									}
								}

								window_posn += touint(match_length);

								while (match_length-- > 0)
									window[rundest++] = window[runsrc++];
							}
						}
						break;
					}
					case LzxConstants::BLOCKTYPE::ALIGNED: {
						while (this_run > 0) {
							main_element = toint(ReadHuffSym(m_state.MAINTREE_table, m_state.MAINTREE_len, LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS, bitbuf));

							if (main_element < LzxConstants::NUM_CHARS) {
								window[window_posn++] = tobyte(main_element);
								this_run--;
							}
							else {
								main_element -= LzxConstants::NUM_CHARS;
								match_length = main_element & LzxConstants::NUM_PRIMARY_LENGTHS;

								if (match_length == LzxConstants::NUM_PRIMARY_LENGTHS) {
									length_footer = toint(ReadHuffSym(m_state.LENGTH_table, m_state.LENGTH_len, LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS, bitbuf));
									match_length += length_footer;
								}
								match_length += LzxConstants::MIN_MATCH;
								match_offset = main_element >> 3;

								if (match_offset > 2) {
									extra = extra_bits[match_offset];
									match_offset = toint(position_base[match_offset]) - 2;

									if (extra > 3) {
										extra -= 3;
										verbatim_bits = toint(bitbuf.ReadBits(tobyte(extra)));
										match_offset += (verbatim_bits << 3);
										aligned_bits = toint(ReadHuffSym(m_state.ALIGNED_table, m_state.ALIGNED_len, LzxConstants::ALIGNED_MAXSYMBOLS, LzxConstants::ALIGNED_TABLEBITS, bitbuf));
										match_offset += aligned_bits;
									}
									else if (extra == 3) {
										aligned_bits = toint(ReadHuffSym(m_state.ALIGNED_table, m_state.ALIGNED_len, LzxConstants::ALIGNED_MAXSYMBOLS, LzxConstants::ALIGNED_TABLEBITS, bitbuf));
										match_offset += aligned_bits;
									}
									else if (extra > 0) {
										verbatim_bits = toint(bitbuf.ReadBits(tobyte(extra)));
										match_offset += verbatim_bits;
									}
									else {
										match_offset = 1;
									}

									R2 = R1; R1 = R0; R0 = touint(match_offset);
								}
								else if (match_offset == 0) {
									match_offset = toint(R0);
								}
								else if (match_offset == 1) {
									match_offset = toint(R1);
									R1 = R0; R0 = touint(match_offset);
								}
								else {
									match_offset = toint(R2);
									R2 = R0; R0 = touint(match_offset);
								}

								rundest = toint(window_posn);
								this_run -= match_length;

								if (window_posn >= match_offset) {
									runsrc = rundest - match_offset;
								}
								else {
									runsrc = rundest + (toint(window_size) - match_offset);
									copy_length = match_offset - toint(window_posn);

									if (copy_length < match_length) {
										match_length -= copy_length;
										window_posn += touint(copy_length);

										while (copy_length-- > 0)
											window[rundest++] = window[runsrc++];

										runsrc = 0;
									}
								}

								window_posn += touint(match_length);

								while (match_length-- > 0)
									window[rundest++] = window[runsrc++];
							}
						}
						break;
					}

					case LzxConstants::BLOCKTYPE::UNCOMPRESSED: {
						if ((inData->Position() + this_run) > endpos)
							return -1;

						std::vector<csbyte> temp_buffer(this_run);
						inData->Read(temp_buffer, 0, this_run);

						const auto size = temp_buffer.size();
						for (size_t i = 0; i < size; ++i) {
							window[toint(window_posn) + i] = temp_buffer[i];
						}

						window_posn += touint(this_run);
						break;
					}
					default:
						return -1;
					}					
				}
			}

			if (togo != 0)
				return -1;
			
			csint start_window_pos = toint(window_posn);
			
			if (start_window_pos == 0)
				start_window_pos = toint(window_size);
			
			start_window_pos -= outLen;
			
			outData.Write(window, start_window_pos, outLen);

			m_state.window_posn = window_posn;
			m_state.R0 = R0;
			m_state.R1 = R1;
			m_state.R2 = R2;
			
			if ((m_state.frames_read++ < 32768) && m_state.intel_filesize != 0) {
				if (outLen <= 6 || m_state.intel_started == 0) {
					m_state.intel_curpos += outLen;
				}
				else {
					csint dataend = outLen - 10;
					csuint curpos = touint(m_state.intel_curpos);

					m_state.intel_curpos = toint(curpos) + outLen;

					while (outData.Position() < dataend) {
						if (outData.ReadByte() != 0xE8)
							curpos++; continue;
					}
				}

				return -1;
			}

			return 0;
		}

	private:
		LzxState m_state;

		static constexpr void fillExtraBits() {
			if (extra_bits.empty()) {
				extra_bits.resize(52);

				for (csint i = 0, j = 0; i <= 50; i += 2) {
					extra_bits[i] = extra_bits[i + 1] = tobyte(j);

					if ((i != 0) && (j < 17))
						j++;
				}
			}
		}

		static constexpr void fillPositionBase() {
			if (position_base.empty())
			{
				position_base.resize(51);

				for (csint i = 0, j = 0; i <= 50; i++) {
					position_base[i] = touint(j);
					j += 1 << extra_bits[i];
				}
			}
		}

		csint MakeDecodeTable(csuint nsyms, csuint nbits, std::vector<csbyte>& length, std::vector<csushort>& table) {
			csushort sym = 0;
			csuint leaf = 0;
			csbyte bit_num = 1;
			csuint fill = 0;
			csuint pos = 0;
			csuint table_mask = touint(1 << toint(nbits));
			csuint bit_mask = table_mask >> 1;
			csuint next_symbol = bit_mask;
			
			while (bit_num <= nbits) {
				for (sym = 0; sym < nsyms; sym++) {
					if (length[sym] == bit_num)	{
						leaf = pos;

						if ((pos += bit_mask) > table_mask) 
							return 1;
						
						fill = bit_mask;
						
						while (fill-- > 0)
							table[leaf++] = sym;
					}
				}

				bit_mask >>= 1;
				bit_num++;
			}
			
			if (pos != table_mask) {
				
				for (sym = toushort(pos); sym < table_mask; sym++)
					table[sym] = 0;
				
				pos <<= 16;
				table_mask <<= 16;
				bit_mask = 1 << 15;

				while (bit_num <= 16) {
					for (sym = 0; sym < nsyms; sym++) {
						if (length[sym] == bit_num)	{
							leaf = pos >> 16;
							for (fill = 0; fill < bit_num - nbits; fill++) {								
								if (table[leaf] == 0) {
									table[(next_symbol << 1)] = 0;
									table[(next_symbol << 1) + 1] = 0;
									table[leaf] = toushort(next_symbol++);
								}
								
								leaf = touint(table[leaf] << 1);
								
								if (((pos >> toint(15 - fill)) & 1) == 1)
									leaf++;
							}

							table[leaf] = sym;

							if ((pos += bit_mask) > table_mask)
								return 1;
						}
					}

					bit_mask >>= 1;
					bit_num++;
				}
			}
			
			if (pos == table_mask)
				return 0;
			
			for (sym = 0; sym < nsyms; sym++) {
				if (length[sym] != 0)
					return 1;
			}			

			return 0;
		}

		void ReadLengths(std::vector<csbyte>& lens, csuint first, csuint last, BitBuffer& bitbuf) {
			csuint x, y;
			csint z;			

			for (x = 0; x < 20; x++) {
				y = bitbuf.ReadBits(4);
				m_state.PRETREE_len[x] = tobyte(y);
			}
			
			MakeDecodeTable(LzxConstants::PRETREE_MAXSYMBOLS, LzxConstants::PRETREE_TABLEBITS, m_state.PRETREE_len, m_state.PRETREE_table);

			for (x = first; x < last;) {
				z = toint(ReadHuffSym(m_state.PRETREE_table, m_state.PRETREE_len, LzxConstants::PRETREE_MAXSYMBOLS, LzxConstants::PRETREE_TABLEBITS, bitbuf));
				
				if (z == 17) {
					y = bitbuf.ReadBits(4); 
					y += 4;
					
					while (y-- != 0) 
						lens[x++] = 0;
				}
				else if (z == 18) {
					y = bitbuf.ReadBits(5); 
					y += 20;
					
					while (y-- != 0)
						lens[x++] = 0;
				}
				else if (z == 19) {
					y = bitbuf.ReadBits(1); y += 4;
					z = toint(ReadHuffSym(m_state.PRETREE_table, m_state.PRETREE_len, LzxConstants::PRETREE_MAXSYMBOLS, LzxConstants::PRETREE_TABLEBITS, bitbuf));
					z = lens[x] - z;
					
					if (z < 0) 
						z += 17;
					
					while (y-- != 0) 
						lens[x++] = tobyte(z);
				}
				else
				{
					z = lens[x] - z; 
					
					if (z < 0) 
						z += 17;

					lens[x++] = tobyte(z);
				}
			}
		}

		csuint ReadHuffSym(std::vector<csushort>& table, std::vector<csbyte>& lengths, csuint nsyms, csuint nbits, BitBuffer& bitbuf) {
			csuint i, j;
			bitbuf.EnsureBits(16);

			if ((i = table[bitbuf.PeekBits(tobyte(nbits))]) >= nsyms) {
				j = touint(1 << toint((sizeof(csuint) * 8) - nbits));
				do
				{
					j >>= 1; 
					i <<= 1;
					i |= (bitbuf.GetBuffer() & j) != 0 ? touint(1) : 0;

					if (j == 0) 
						return 0;

				} while ((i = table[i]) >= nsyms);
			}

			j = lengths[i];
			bitbuf.RemoveBits(tobyte(j));

			return i;
		}
	};
}

#endif