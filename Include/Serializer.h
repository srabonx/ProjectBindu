#pragma once

#include <fstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>



namespace BINDU
{

	class Serializer
	{
	public:


		template <typename T>
		void SerializeToXML(const std::string& name, const std::string& filename, const T& data)
		{
			cereal::XMLOutputArchive::Options options;
			options.indent(true);


			std::ofstream os(filename);
			cereal::XMLOutputArchive archive(os, options);

			archive(cereal::make_nvp(name, data));
		}

		template <typename T>
		void SerializeToBinary(const std::string& name, const std::string& filename, const T& data)
		{
			std::ofstream os(filename, std::ios::binary);
			cereal::BinaryOutputArchive archive(os);

			archive(cereal::make_nvp(name, data));
		}

		template<typename T>
		void DeserializeFromXML(const std::string& name, const std::string& filename, T& data)
		{
			std::ifstream is(filename);

			cereal::XMLInputArchive archive(is);

			archive(cereal::make_nvp(name, data));
		}

		template<typename T>
		void DeserializeFromBinary(const std::string& name, const std::string& filename, T& data)
		{
			std::ifstream is(filename, std::ios::binary);

			cereal::BinaryInputArchive archive(is);

			archive(cereal::make_nvp(name, data));
		}

	};
}
